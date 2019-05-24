import tensorflow as tf
from tensorflow.keras import backend as K
import os


def save_protobuf(model, directory, filename):
    frozen_graph = freeze_session(K.get_session(), output_names=[out.op.name for out in model.outputs])
    tf.train.write_graph(frozen_graph, directory, filename, as_text=False)
    print("Saved protobuf model to", filename)


def save_json_h5(model, json_file, h5_file):
    model_json = model.to_json()
    with open(json_file, "w") as json_file:
        json_file.write(model_json)
    # serialize weights to HDF5
    model.save_weights(h5_file)
    print("Saved model to", json_file, ",", h5_file)


def save_docker(model, directory, version_number):

    export_path = os.path.join(directory, str(version_number))
    print('Docker model export_path = {}\n'.format(export_path))
    if os.path.isdir(export_path):
        raise Exception('Unable to save model for Docker, directory already exists:' + export_path +
                        '. Is the version number unique?')

    tf.saved_model.simple_save(
        tf.keras.backend.get_session(),
        export_path,
        inputs={'input_image': model.input},
        outputs={t.name: t for t in model.outputs})
    print('Saved Docker model to', export_path)


# From https://stackoverflow.com/questions/45466020/how-to-export-keras-h5-to-tensorflow-pb
def freeze_session(session, keep_var_names=None, output_names=None, clear_devices=True):
    """
    Freezes the state of a session into a pruned computation graph.

    Creates a new computation graph where variable nodes are replaced by
    constants taking their current value in the session. The new graph will be
    pruned so subgraphs that are not necessary to compute the requested
    outputs are removed.
    @param session The TensorFlow session to be frozen.
    @param keep_var_names A list of variable names that should not be frozen,
                          or None to freeze all the variables in the graph.
    @param output_names Names of the relevant graph outputs.
    @param clear_devices Remove the device directives from the graph for better portability.
    @return The frozen graph definition.
    """
    graph = session.graph
    with graph.as_default():
        freeze_var_names = list(set(v.op.name for v in tf.global_variables()).difference(keep_var_names or []))
        output_names = output_names or []
        output_names += [v.op.name for v in tf.global_variables()]
        input_graph_def = graph.as_graph_def()
        if clear_devices:
            for node in input_graph_def.node:
                node.device = ""
        frozen_graph = tf.graph_util.convert_variables_to_constants(
            session, input_graph_def, output_names, freeze_var_names)
        return frozen_graph

from tensorflow import keras
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Dropout, Flatten
from tensorflow.keras.layers import Conv2D, MaxPooling2D
from tensorflow.keras.optimizers import SGD


def get_model():
    model = Sequential(name="input")
    model.add(Conv2D(64, kernel_size=(3, 3), activation='relu', padding='same', input_shape=(4, 4, 1)))
    model.add(Conv2D(64, kernel_size=(3, 3), activation='relu', padding='same'))
    model.add(Conv2D(64, kernel_size=(3, 3), activation='relu', padding='same'))
    model.add(MaxPooling2D(pool_size=(2, 2), padding='same'))

    model.add(Flatten())
    model.add(Dense(256, activation='relu'))
    model.add(Dense(256, activation='relu'))
    model.add(Dense(1, activation='linear', name="finalout"))

    sgd = SGD(lr=0.01, decay=1e-6, momentum=0.9, nesterov=True)
    model.compile(loss='mean_squared_error', optimizer='adam', metrics=['accuracy'], )
    input_tensor = model.inputs[0]
    output_tensor = model.outputs[0]
    print("Inputs: "+str(input_tensor))
    print("Outputs: "+str(output_tensor))
    return model

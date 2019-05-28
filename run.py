import configparser
import re
import subprocess
import python.train
import time
import shutil

config = configparser.ConfigParser()
config.read('settings.ini')
docker_start_port = config.getint('docker', 'start_port')
num_threads = config.getint('control', 'num_threads')


with open('docker-compose.yml', 'r') as f:
    contents = f.read()
pattern = r'(ports:\s*-\s*")(\d+)(-)(\d+):'
replacement = r'\g<1>' + str(docker_start_port) + r'\g<3>' + str(docker_start_port + num_threads - 1) + ':'
contents = re.sub(pattern, replacement, contents)
with open('docker-compose.yml', 'w') as f:
    f.write(contents)

try:
    print("Deleting game_logs dir")
    shutil.rmtree('game_logs')
except FileNotFoundError:
    pass

try:
    print("Deleting models/docker dir")
    shutil.rmtree('models/docker')
except FileNotFoundError:
    pass


subprocess.run('docker-compose down', shell=True)
subprocess.run('docker-compose up -d --scale tensorflow_serving=' + str(num_threads), shell=True)

train = python.train.Train()
# give time for docker to update
time.sleep(1)
print('\n=============================== deeptile ' + str(0) + ' ==============================')
subprocess.run('./deeptile', shell=True)

for run in range(1, 10):
    print('\n=============================== train ' + str(run) + ' ==============================')
    train.iterate()

    # give time for docker to update
    time.sleep(1)
    print('\n=============================== deeptile ' + str(run) + ' ==============================')
    subprocess.run('./deeptile', shell=True)

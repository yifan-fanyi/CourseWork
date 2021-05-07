import os
import numpy as np
from keras.models import load_model

from keras.preprocessing.image import ImageDataGenerator
from keras.utils.data_utils import get_file

VERSION=1.1
batch_size = 4

#### CLIP-BEGIN
#### DO NOT MODIFY ABOVE THIS LINE

modelUrl = 'https://s3.amazonaws.com/ee599yifan/weights-improvement-03-0.41.hdf5'

test_datagen = ImageDataGenerator(
    rescale=1./255)

test_generator = test_datagen.flow_from_directory(
    './data-test', # DO NOT EDIT THIS LINE
    color_mode='grayscale',
    target_size=(128, 128),
    batch_size=batch_size, # DO NOT EDIT THIS LINE
    class_mode='categorical')

## add some more content here

#### DO NOT MODIFY BELOW THIS LINE
#### CLIP-END

print('Download model weights from %s' % (modelUrl))
modelFile = get_file(os.path.basename(modelUrl), modelUrl)
print('Download complete -- %d bytes' % (os.path.getsize(modelFile)))

model = load_model(modelFile)
print('SUMMARY')
print(model.summary())

y = model.predict_generator(test_generator, steps=test_generator.n/batch_size, verbose=0)
pred = np.argmax(y, axis=1)
print('Processed %d test inputs' % (len(y)))

from collections import defaultdict

class_correct = defaultdict(int)
class_count = defaultdict(int)

class_map = {v: k for k, v in test_generator.class_indices.items()}

for k in range(len(pred)):
    cls = class_map[ test_generator.classes[k] ]
    class_correct[cls] += ( pred[k] == test_generator.classes[k] )
    class_count[cls] += 1

acc = sum(class_correct.values()) / sum(class_count.values())
acc_by_class = {k:class_correct[k] / class_count[k] for k in class_count.keys()}

import json
output = {
    "scores": {}
}

output['scores']['Accuracy'] = '%.3f' % (100*acc)
for k,v in acc_by_class.items():
    output['scores']['Accuracy (%s)' % (k)] = '%.3f' % (100*v)
output['scores']['Parameter count'] = model.count_params()

output['scores']['Average accuracy'] = '%.3f' % (100*np.average( list(acc_by_class.values()) ))

output['scoreboard'] = [
    '%.3f' % (100*acc),
    '%.3f' % (100*np.average( list(acc_by_class.values()) )),
    model.count_params()
]

print(json.dumps(output))

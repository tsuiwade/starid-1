### *cnn2* cnn using libstarid
###

import random
import numpy as np
import tensorflow as tf
import libstarid.libstarid as ls
libstarid = ls.libstarid()

w1 = tf.Variable(tf.truncated_normal([5, 5, 1, 32], stddev=0.1), dtype=tf.float32)
b1 = tf.Variable(tf.constant(0.1, shape=[32]), dtype=tf.float32)
w2 = tf.Variable(tf.truncated_normal([5, 5, 32, 64], stddev=0.1), dtype=tf.float32)
b2 = tf.Variable(tf.constant(0.1, shape=[64]), dtype=tf.float32)
w3 = tf.Variable(tf.truncated_normal([7*7*64, 1024], stddev=0.1), dtype=tf.float32)
b3 = tf.Variable(tf.constant(0.1, shape=[1024]), dtype=tf.float32)
w4 = tf.Variable(tf.truncated_normal([1024, 10], stddev=0.1), dtype=tf.float32)
b4 = tf.Variable(tf.constant(0.1, shape=[10]), dtype=tf.float32)

def inference(images):
    conv1 = tf.nn.conv2d(images, w1, strides=[1, 1, 1, 1], padding='SAME') + b1
    pool1 = tf.nn.max_pool(tf.nn.relu(conv1), ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')
    conv2 = tf.nn.conv2d(pool1, w2, strides=[1, 1, 1, 1], padding='SAME') + b2
    pool2 = tf.nn.max_pool(tf.nn.relu(conv2), ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1], padding='SAME')
    full3 = tf.matmul(tf.reshape(pool2, [-1, 7*7*64]), w3) + b3
    drop3 = tf.nn.dropout(tf.nn.relu(full3), 1.0)
    return tf.matmul(drop3, w4) + b4

def inputs(batch_size):
    images = np.zeros((batch_size, 28, 28, 1), dtype=np.float32)
    labels = np.zeros((batch_size), dtype=np.int32)
    for cnt in range(0, batch_size):
        starndx = random.randint(0, 9)
        images[cnt, :, :, 0] = libstarid.image(starndx=starndx)
        labels[cnt] = starndx
    return images, labels

def evaluate(batch_size):
    images, labels = inputs(batch_size)
    predicted = tf.cast(tf.arg_max(inference(images), 1), tf.int32)
    return tf.reduce_mean(tf.cast(tf.equal(predicted, labels), tf.float32))

images, labels = inputs(batch_size=100)
logits = inference(images)
loss = tf.reduce_mean(tf.nn.sparse_softmax_cross_entropy_with_logits(logits=logits, labels=labels))
train = tf.train.AdamOptimizer(1e-4).minimize(loss)
accuracy = evaluate(batch_size=100)
steps = 200
saver = tf.train.Saver()
coord = tf.train.Coordinator()
with tf.Session() as sess:
    tf.global_variables_initializer().run()
    threads = tf.train.start_queue_runners(sess=sess, coord=coord)
    for step in range(0, steps)
        sess.run(train)
        if step % 10 == 0:
            print('step %d loss %3.2f accuracy %3.2f' % (step, sess.run(loss), sess.run(accuracy)))
    saver.save(sess, 'data_cnn2/model', global_step=steps)
coord.request_stop()
coord.join(threads)
sess.close()
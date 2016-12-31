from datetime import datetime
import time
import tensorflow as tf
import tfrecords as tr
import graph as gn
FLAGS = tf.app.flags.FLAGS
tf.app.flags.DEFINE_string('learn_dir', '/home/noah/dev/learn', '')
tf.app.flags.DEFINE_string('data_dir', '/home/noah/dev/starid/data', '')
tf.app.flags.DEFINE_string('learn_data', 'images_a.tfrecords', '')
tf.app.flags.DEFINE_string('predict_data', 'images_b.tfrecords', '')
tf.app.flags.DEFINE_integer('batch_size', 100, '')
tf.app.flags.DEFINE_integer('max_steps', 590, '')
# noah@noah:~/dev/starid$ tensorboard --logdir=graphnodes/train &

def train():
  with tf.Graph().as_default():
    global_step = tf.Variable(0, trainable=False)

    images, labels = tr.inputs_learn(FLAGS)
    softmax = gn.inference(images)
    cost = gn.cost(softmax, labels)
    learn = tf.train.AdamOptimizer(1e-4).minimize(cost)
    init = tf.global_variables_initializer()
    img = tf.summary.image('test', images)
    saver = tf.train.Saver()

    sess = tf.Session()
    sess.run(init)
    summary = tf.summary.merge_all()
    summary_writer = tf.summary.FileWriter(FLAGS.learn_dir, sess.graph)
    tf.train.start_queue_runners(sess=sess)

    for step in range(590):
      start_time = time.time()
      _, cost_value, img_summary = sess.run([learn, cost, img])
      duration = time.time() - start_time
      if step % 10 == 0:
        num_examples_per_step = FLAGS.batch_size
        examples_per_sec = num_examples_per_step / duration
        sec_per_batch = float(duration)
        format_str = ('%s: step %d, loss = %.2f (%.1f examples/sec; %.3f sec/batch)')
        print (format_str % (datetime.now(), step, cost_value, examples_per_sec, sec_per_batch))
      if step % 100 == 0:
        summary_str = sess.run(summary)
        summary_writer.add_summary(summary_str, step)
        summary_writer.add_summary(img_summary)
    saver.save(sess, '/home/noah/dev/learn/model.ckpt')

def main(argv=None):
  if tf.gfile.Exists(FLAGS.learn_dir):
    tf.gfile.DeleteRecursively(FLAGS.learn_dir)
  tf.gfile.MakeDirs(FLAGS.learn_dir)
  train()

if __name__ == '__main__':
  tf.app.run()
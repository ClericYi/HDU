import tensorflow as tf

v1 = tf.Variable(tf.constant(1.0, shape=[1]), name='v1')
v2 = tf.Variable(tf.constant(2.0, shape=[1]), name='v2')
result = tf.add(v1, v2, name='add')

sess = tf.compat.v1.Session()
writer = tf.summary.FileWriter("logs/", sess.graph)

init = tf.global_variables_initializer()
sess.run(init)

graph_def = tf.get_default_graph().as_graph_def()

output_graph_def = tf.compat.v1.graph_util.convert_variables_to_constants(sess, graph_def, ['add'])
with tf.io.gfile.GFile('model.pb', 'wb') as f:
    f.write(output_graph_def.SerializeToString())

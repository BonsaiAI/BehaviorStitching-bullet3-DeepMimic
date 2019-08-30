import tensorflow as tf

sess = tf.InteractiveSession()

raw_data = [1., 2., 8, -1, 0, 5.5, 6, 13]
spike = tf.Variable(False)
spike.initializer.run()

for i in range(1, len(raw_data)):
    if raw_data[i] - raw_data[i-1] > 5:
        updater = tf.assign(spike, True)
        updater.eval()
    else:
        tf.assign(spike, False).eval()
    print("Spike", spike.eval())

sess.close()

# import tensorflow as tf
#
# x=tf.constant([[1., 2.]])
# negMatrix = tf.negative(x)
#
# with tf.Session(config=tf.ConfigProto(log_device_placement=True)) as sess:
#     result = sess.run(negMatrix)
#
# print(result)

#################################3333333333
# import tensorflow as tf
#
# sess = tf.interactivesession()
#
# x = tf.constant([[1., 2.]])
# negmatrix = tf.negative(x)
#
# result = negmatrix.eval()
# print(result)
#
# sess.close()
###############################
# import tensorflow as tf
#
# x=tf.constant([[1., 2.]])
# negmatrix = tf.negative(x)
#
# with tf.session() as sess:
#     result = sess.run(negmatrix)
#
# print(result)

# #####################
#
# import tensorflow as tf
# import numpy as np
#
# m1 = [[1.0, 2.0],
#       [3.0, 4.0]]
#
# m2 = np.array(m1, dtype=np.float32)
#
# m3 = tf.constant(m1)
#
# print(type(m1))
# print(type(m2))
# print(type(m3))
#
#
# t1 = tf.convert_to_tensor(m1, dtype=tf.float32)
# t2 = tf.convert_to_tensor(m2, dtype=tf.float32)
# t3 = tf.convert_to_tensor(m3, dtype=tf.float32)
#
# print(type(t1))
# print(type(t2))
# print(type(t3))
#
# print(t3)
#
# t4 = tf.ones([500, 500])*0.5
# print(t4)
#


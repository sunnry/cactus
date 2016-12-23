a example of showing how to use arduino mega to control esc.

in this example, the key function is servopulse(), this function convert power to ppm signal,
after testing the minmal power is between 9 to 13, different ecs the minimal power is some different,
so you had better verify before start, there is some experiences that you can use a more bigger power
to start ecs and then convert to minial power, this will be useful to power up esc with minimal power,
otherwise if you use minimal power to start esc directly it will be failed sometimes.

该例子中的关键函数便是servopulse()，该函数把无几电刷的油程转化为PPM信号，经过测试可以稳定启动
的最小量程在9~13之间，不同电刷稍有差异，所以启动前做好需要校准，另外还有一个最小的量程启动经验，
先用一个稍大的量程启动，待启动后再切换到最小量程，这样便于最小量程启动，否则有时候不同电刷由于
相位的稍许差别，便无法成功启动。
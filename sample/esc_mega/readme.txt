a example of showing how to use arduino mega to control esc.

in this example, the key function is servopulse(), this function convert power to ppm signal,
after testing the minmal power is between 9 to 13, different ecs the minimal power is some different,
so you had better verify before start, there is some experiences that you can use a more bigger power
to start ecs and then convert to minial power, this will be useful to power up esc with minimal power,
otherwise if you use minimal power to start esc directly it will be failed sometimes.

�������еĹؼ���������servopulse()���ú������޼���ˢ���ͳ�ת��ΪPPM�źţ��������Կ����ȶ�����
����С������9~13֮�䣬��ͬ��ˢ���в��죬��������ǰ������ҪУ׼�����⻹��һ����С�������������飬
����һ���Դ�����������������������л�����С���̣�����������С����������������ʱ��ͬ��ˢ����
��λ�������𣬱��޷��ɹ�������
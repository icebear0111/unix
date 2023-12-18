fd = open("data1", O_RDWR|O_CREAT, 0600);

lseek(fd, 0, SEEK_SET);

lseek(fd, 1, SEEK_END);

i = 0; i < 4; i++
lseek(fd, i * 2 + 1, SEEK_SET);

lseek(fd, 15, SEEK_SET);

lseek(fd, -2, SEEK_END);

lseek(fd, -2, SEEK_CUR);

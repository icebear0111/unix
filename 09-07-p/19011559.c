fd = open("test1", O_WRONLY|O_CREAT, 0600); 
write(fd, &ch, 1);

fd = open("test2", O_WRONLY|O_CREAT, 0640);
write(fd, &data, sizeof(int));

fd = open("test1", O_WRONLY);
write(fd, &ch, 1);

fd = open("test1", O_WRONLY|O_APPEND);
write(fd, &ch, 1);

fd = open("test1", O_WRONLY|O_TRUNC);
write(fd, &ch, 1);

fd = open("test2", O_RDONLY);
read(fd, data, 20);

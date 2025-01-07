    perror("err opening file");
    return NULL;
  }

  void * buff = malloc(buf.st_size);

  if (buff == NULL)
  {
    perror("error allocating buffer");
    return NULL;
  }

  size_t count = 0;
  while (count < buf.st_size)
  {
    int bytes;
    if (( bytes = read(fd, buff, buf.st_size))!= -1)
    {
      count += bytes;
    }
    else
    {
      perror("error reading");
      close(fd);
      free(buff);
      return NULL;
    }
  }

  printf("%ld bytes written\n", count);
  *size = count;
  close(fd);
  return buff;
}


int main(int argc, char* argv[])
{
  int c;
  char* filename;

  if (argc<2)
  {
    errno = EIO;
    perror("No file provided");
    return -1;
  }
  else
  {
    filename = argv[1];
  }

  size_t size;
  char* buff = (char*) readInputFile(filename, &size);

  if (buff == NULL)
  {
    return -1;
  }

  vector<variable_t> vars = populateVariable(buff, size);

  //example

  printf("%d\n", ModifyData(buff, size, vars, 0, 0, 1, 0x8));

  free(buff);
  return 0;
}

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <vector>
#include <zlib.h>


using namespace std;

typedef struct {
  u_int32_t off;
  u_int32_t var_sz;
  u_int32_t data_sz;
  u_int32_t crc32;
  u_int32_t sz;
} variable_t;


u_int32_t calculateCrc32(char* buff, variable_t& var)
{
   return crc32(0x0, (const Bytef*) &buff[var.off], var.sz-4);
}


int ModifyData (char* buff, u_int32_t sz, vector<variable_t>& vars, u_int32_t varNo, u_int32_t dataOffset, u_int32_t dataSize, u_int32_t newVal)
{
  if (varNo >= vars.size() )
  {
    printf("Err: VarNo exceeds bound\n");
    return -1;
  }

  if (dataOffset+dataSize > vars[varNo].data_sz)
  {
    printf("Err: Wrong Data SizE\n");
    return -1;
  }


  int count  = 0;

  while (count < dataSize)
  {
    *((u_int8_t*) &buff[vars[varNo].off + 8 + vars[varNo].var_sz + 16 + 4 + dataOffset+ count]) = (newVal>>(count*8)) & 0xFF ;
    count++;
  }

  printf("val: %x\n", *(u_int8_t*) &buff[vars[varNo].off + 8 + vars[varNo].var_sz + 16 + 4 + dataOffset]);

  *((u_int32_t*) &buff[vars[varNo].sz-4]) =  calculateCrc32(buff, vars[varNo]);


  int fd = open("out.txt", O_WRONLY|O_CREAT, S_IRWXU);

  if (fd < 0)
  {
    perror("Can not open output file");
    return -1;
  }

  count = 0;
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

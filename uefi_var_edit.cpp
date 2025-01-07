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

  while (count< sz)
  {
    int bytes = 0;
    if ( (bytes = write(fd, buff, sz)) != -1)
    {
      count += bytes;
    }
    else
    {
      perror("Can not write!");
      close(fd);
      return -1;
    }
  }
  close(fd);
  return 0;
}



vector<variable_t> populateVariable(char* buff, int size)
{
  vector<variable_t> res;
  size_t count = 0;
  while(count < size)
  {
    variable_t temp;
    temp.off = count;
    temp.var_sz = * (u_int32_t*) &buff[temp.off];
    temp.data_sz = *(u_int32_t*) &buff[count+4];
    temp.crc32 =  *(u_int32_t*) &buff[temp.off+8+temp.var_sz+16+4+temp.data_sz];
    temp.sz = 8 + temp.var_sz + 16 + 4 + temp.data_sz+4;// 8 (var_sz+data_sz) + var + GUID + Attr + data + CRC32
    count = (temp.off + temp.sz);
    res.push_back(temp);
    printf("offset = 0x%x, var_sz = %x, data_sz = %x, crc32 = 0x%x\n", temp.off, temp.var_sz, temp.data_sz, temp.crc32);
  }

  return res;
}

void* readInputFile(char* filename, size_t* size)
{
  struct stat buf;
  printf("Filename: %s\n", filename);
  if (stat((const char*)filename, &buf) != 0)
  {
    perror("err stat file");
    return NULL;
  }
  printf("Size: 0x%lx\n", buf.st_size);
  int fd = open(filename, O_RDONLY);

  if (fd <0 )
  {
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

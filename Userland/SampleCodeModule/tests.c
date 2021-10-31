#include <tests.h>

#define MAX_BLOCKS 128
#define MAX_MEMORY 1024 //Should be around 80% of memory managed by the MM

typedef struct MM_rq{
  void *address;
  uint32_t size;
}mm_rq;

void testMM(){
  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;

  while (1) {
    rq = 0;
    total = 0;

    // Request as many blocks as we can
    while(rq < MAX_BLOCKS && total < MAX_MEMORY){
      mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
      mm_rqs[rq].address = alloc(mm_rqs[rq].size); // TODO: Port this call as required
//TODO: check if NULL
      total += mm_rqs[rq].size;
      rq++;
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        memset(mm_rqs[i].address, i, mm_rqs[i].size); // TODO: Port this call as required

    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        if(!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
          print(1, "ERROR!\n", 8); // TODO: Port this call as required


    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        free(mm_rqs[i].address);  // TODO: Port this call as required

    print(1, "To stop testing press ENTER or TAB, to continue, press any other key\n", 70);
    int c = get_char();
    if (c == '\n' || c == -1) {
      put_char(1, '\n');
      return;
    }
  } 
}

/*
 * mm-implicit.c
 * 
 * code is based on CSAPP 3e textbook section 9.9
 */

#define SLOCAL

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdint.h>

#include "memlib.h"
#include "mm.h"

static char *heap_listp, *link_head;

/* do not change the following! */
#ifdef DRIVER
/* create aliases for driver tests */
#define malloc mm_malloc
#define free mm_free
#define realloc mm_realloc
#define calloc mm_calloc
#endif /* def DRIVER */

/* Basic constants and macros */
#define WSIZE 4             /* Word and header/footer size (bytes) */
#define DSIZE 8             /* Double word size (bytes) */
#define CHUNKSIZE (1 << 12) /* Extend heap by this amount (bytes) */

#define MAX(x, y) ((x) > (y) ? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))
#define PUT_PTR(p, ptr) PUT(p, (uintptr_t)(ptr-heap_listp))
#define PUT_SUCC(p, ptr) PUT_PTR(p, ptr)
#define PUT_NEXT(p, ptr) PUT_PTR(p+WSIZE, ptr)
#define EMPTY_E_NODE (heap_listp)

/* Read the size and allocated fields from address p */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x3)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *)(bp)-WSIZE)
#define FTRP(bp) ((char *)(bp)+GET_SIZE(HDRP(bp))-DSIZE)
#define SUCC(bp) (GET(bp)+EMPTY_E_NODE)
#define NEXT(bp) (GET(bp+WSIZE)+EMPTY_E_NODE)

/* Given block ptr bp, compute address of next and previous blocks */

#define GET_SIZE_BP(bp) GET_SIZE(HDRP(bp))
#define GET_ALLOC_BP(bp) GET_ALLOC(HDRP(bp)) 

#define NEXT_BLKP(bp) ((char *)(bp)+GET_SIZE_BP((char *)(bp)))
#define PREV_BLKP(bp) ((char *)(bp)-GET_SIZE(((char *)(bp)-DSIZE)))

static void *extend_heap(size_t words);
static void *coalesce(void *bp);
static void *find_fit(size_t asize);
static void place(void *bp, size_t asize);

#ifdef LOCAL
static int oper_count=0;
static int link_length=0;
#endif
/*
 * mm_init
 */

void ddprintf(char* fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fflush(stderr);
}

int mm_init(void)
{
    /* Create the initial empty heap */
    if ((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1)
        return -1;
    PUT(heap_listp, 0);                     /* Alignment padding */
    PUT(heap_listp + (1*WSIZE), PACK(DSIZE, 1));  /* Prologue header */
    PUT(heap_listp + (2*WSIZE), PACK(DSIZE, 1));  /* Prologue footer */
    PUT(heap_listp + (3*WSIZE), PACK(0, 3));      /* Epilogue header */
    heap_listp += (2*WSIZE);
    link_head = EMPTY_E_NODE;

    /* Extend the empty heap with a free block of CHUNKSIZE bytes */
    // if (extend_heap(CHUNKSIZE/WSIZE) == NULL)
        // return -1;
    return 0;
}

/*
 * malloc
 */
void erase_node(char* bp){
    #ifdef LOCAL
    link_length--;
    #endif
    char* succ = SUCC(bp);
    char* next = NEXT(bp);
    if(succ!=EMPTY_E_NODE)
        PUT_NEXT(succ, next);
    else
        link_head=next;
    if(next!=EMPTY_E_NODE)
        PUT_SUCC(next, succ);
    #ifdef LOCAL
    assert(link_head==EMPTY_E_NODE||SUCC(link_head)==EMPTY_E_NODE);
    #endif
}

void move_node(char* bp, char* nbp){
    char* succ = SUCC(bp);
    char* next = NEXT(bp);
    if(succ!=EMPTY_E_NODE)
        PUT_NEXT(succ, nbp);
    else
        link_head=nbp;
    if(next!=EMPTY_E_NODE)
        PUT_SUCC(next, nbp);
    PUT_SUCC(nbp, succ);
    PUT_NEXT(nbp, next);
    #ifdef LOCAL
    assert(link_head==EMPTY_E_NODE||SUCC(link_head)==EMPTY_E_NODE);
    #endif
}

void new_node(char* bp){
    #ifdef LOCAL
    link_length++;
    #endif
    PUT_SUCC(bp, EMPTY_E_NODE);
    PUT_NEXT(bp, link_head);
    if(link_head!=EMPTY_E_NODE)
        PUT_SUCC(link_head, bp);
    link_head=bp;
    #ifdef LOCAL
    assert(link_head==EMPTY_E_NODE||SUCC(link_head)==EMPTY_E_NODE);
    #endif
}

void *malloc(size_t size) {
    // fprintf(stdout, "malloc(%zu)\n", size);
    size_t asize;       /* Adjusted block size */
    size_t extendsize;  /* Amount to extend heap if no fit */
    char *bp;
    #ifdef LOCAL
    ddprintf("MAC: %d\n", ++oper_count);
    #endif
    /* Ignore spurious requests */
    if (size == 0)
        return NULL;

    /* Adjust block size to include overhead and alignment reqs. */
    if (size <= DSIZE)
        asize = 2*DSIZE;
    else
        asize = DSIZE * ((size + (WSIZE) + (DSIZE-1)) / DSIZE);

    /* Search the free list for a fit */
    if ((bp = find_fit(asize)) != NULL) {
        place(bp, asize);
        #ifdef LOCAL
        ddprintf("ACC: %p->%d\n", bp, asize);
        #endif
        return bp;
    }

    /* No fit found. Get more memory and place the block */
    extendsize = asize;
    // extendsize = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap(extendsize/WSIZE)) == NULL)
        return NULL;
    place(bp, asize);
    #ifdef LOCAL
    ddprintf("ACC: %p->%d\n", bp, asize);
    #endif
    return bp;
}

/*
 * free
 */
void free(void *bp) {
    // fprintf(stdout, "free(%p)\n", bp);
    #ifdef LOCAL
    ddprintf("FRE: %d\n", ++oper_count);
    #endif

    if(bp==NULL)
        return ;
    size_t size = GET_SIZE(HDRP(bp));
    unsigned val=PACK(size, GET(HDRP(bp))&2);
    PUT(HDRP(bp), val);
    PUT(FTRP(bp), PACK(size, 0));
    char *next_bp=NEXT_BLKP(bp);
    PUT(HDRP(next_bp), GET(HDRP(next_bp))&~0x2);
    new_node(bp);
    coalesce(bp);
}

/*
 * realloc
 */
void *realloc(void *oldptr, size_t size) {
    // return oldptr+size;
    // return oldptr+size;
    size_t oldsize, asize;
    // void *newptr, *bp;
    // void *newptr;

    if (size == 0) {
        free(oldptr);
        return NULL;
    }

    if (oldptr == NULL) {
        return malloc(size);
    }

    oldsize = GET_SIZE(HDRP(oldptr));
    /* Adjust block size to include overhead and alignment reqs. */
    if (size <= DSIZE)
        asize = 2 * DSIZE;
    else
        asize = DSIZE * ((size + (WSIZE) + (DSIZE - 1)) / DSIZE);

    if(oldsize >= asize + 2 * DSIZE){
        // memmove(oldptr, oldptr, oldsize - DSIZE);
        // if ((oldsize - asize) >= (2 * DSIZE)) {
            // PUT(HDRP(NEXT_BLKP(bp)), PACK(ori_size-asize, 2));
            // PUT(FTRP(NEXT_BLKP(bp)), PACK(ori_size-asize, 0));

            char *real_next=NEXT_BLKP(oldptr);
            unsigned val=PACK(asize, GET(HDRP(oldptr))&3);
            PUT(HDRP(oldptr), val);
            // PUT(FTRP(oldptr), PACK(asize, 1));
            char *bp = NEXT_BLKP(oldptr);
            
            val=GET(HDRP(real_next))&~0x2;
            PUT(HDRP(real_next), val);
            PUT(HDRP(bp), PACK(oldsize - asize, 2));
            PUT(FTRP(bp), PACK(oldsize - asize, 0));
            new_node(bp);
            // need to coalesce this free block with next block if possible
            coalesce(bp);
        // }
        return oldptr;
    }
    else{
        char *newptr;
        // if(!GET_ALLOC_BP(oldptr) && GET_SIZE_BP(NEXT_BLKP(oldptr)) + oldsize >= asize){
        //     char *next_bp=NEXT_BLKP(oldptr);
        //     char *bp=oldptr;
        //     int newsize=GET_SIZE_BP(NEXT_BLKP(oldptr)) + oldsize;
        //     if(newsize-asize>=2*DSIZE){
        //         PUT(HDRP(bp), PACK(asize, 1));
        //         PUT(FTRP(bp), PACK(asize, 1));
        //         move_node(next_bp, NEXT_BLKP(bp));
        //         PUT(HDRP(NEXT_BLKP(bp)), PACK(newsize-asize, 0));
        //         PUT(FTRP(NEXT_BLKP(bp)), PACK(newsize-asize, 0));
        //     }
        //     else{
        //         erase_node(next_bp);
        //         PUT(HDRP(bp), PACK(newsize, 1));
        //         PUT(FTRP(bp), PACK(newsize, 1));
        //     }
        //     newptr=bp;
        // }
        // else{
            newptr = malloc(size);
            memmove(newptr, oldptr, asize - WSIZE);
            free(oldptr);
        // }
        return newptr;
    }
}

/*
 * calloc
 */
void *calloc(size_t nmemb, size_t size) {
    size_t bytes = nmemb * size;
    void *newptr;

    if ((newptr = malloc(bytes)) != NULL)
        memset(newptr, 0, bytes);
    return newptr;
}

/*
 * mm_checkheap
 */
bool mm_checkheap(int lineno) {
    return (bool)lineno;
}

/* 
 * extend_heap
 * Extend the heap by `words` words. Return a pointer to the new free block on
 * success. Otherwise return NULL.
 * HINT: Make sure the heap size is properly aligned. Don't forget to coalesce free blocks.
*/
static void *extend_heap(size_t words)
{
    char *bp;
    size_t size;
    #ifdef LOCAL
    ddprintf("EXT: %d\n", words);
    #endif
    /* Allocate an even number of words to maintain alignment */
    size = (words % 2) ? (words+1)*WSIZE : words*WSIZE;
    if ((long)(bp = mem_sbrk(size)) == -1)
        return NULL;

    /* Initialize free block header/footer and the epilogue header */
    unsigned val=PACK(size, GET(HDRP(bp))&2);
    PUT(HDRP(bp), val);              /* Free block header */
    PUT(FTRP(bp), PACK(size, 0));              /* Free block footer */
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));      /* New epilogue header */

    /* Coalesce if the previous block was free */
    new_node(bp);
    bp=coalesce(bp);
    return bp;
}

/*
 * coalesce
 * Merge two adjacent free memory chunks, return the merged block.
*/
static void *coalesce(void *bp)
{
    size_t prev_alloc = (GET(HDRP(bp))>>1)&1;
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc) {    /* Case 1 */
        return bp;
    }
    else if (prev_alloc && !next_alloc) {  /* Case 2 */
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        erase_node(NEXT_BLKP(bp));
        erase_node(bp);
        PUT(HDRP(bp), PACK(size, 2));
        PUT(FTRP(bp), PACK(size, 0));
        new_node(bp);
    }
    else if (!prev_alloc && next_alloc) {  /* Case 3 */
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        erase_node(bp);
        erase_node(PREV_BLKP(bp));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 2));
        bp = PREV_BLKP(bp);
        new_node(bp);
    }
    else {  /* Case 4 */
        erase_node(NEXT_BLKP(bp));
        erase_node(bp);
        erase_node(PREV_BLKP(bp));
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) +
                GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 2));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
        new_node(bp);
    }
    return bp;
}

/* First-fit search */
// Return the first fit block, if not find, return NULL
static void *find_fit(size_t asize) {
    // asize: size with footer and header
    // return asize?NULL:NULL;
    int count=0;
    char *target_bp=NULL;
    size_t best_value=0x7fffffff;
    char *cur_bp=link_head;
    while(cur_bp!=EMPTY_E_NODE){
        count++;
        if(GET_SIZE_BP(cur_bp)>=asize){
            if(GET_SIZE_BP(cur_bp)<best_value){
                best_value=GET_SIZE_BP(cur_bp);
                target_bp=cur_bp;
                // break;
            }
            if(count>8){
                break;
            }
            // if(link_length>30000){
                // break;
            // }
        }
        cur_bp=NEXT(cur_bp);
    }
    // ddprintf("%d\n", count);
    return target_bp;

    // char *cur_bp=link_head;
    // while(cur_bp!=EMPTY_E_NODE&&(GET_SIZE_BP(cur_bp)<asize)){
    //     cur_bp=NEXT(cur_bp);
    // }
    // // ddprintf("%d\n", count);
    // return cur_bp!=EMPTY_E_NODE?cur_bp:NULL;
}

// Place the block
static void place(void *bp, size_t asize) {
// Your code here
    unsigned ori_size=GET_SIZE_BP(bp);
    if(ori_size-asize>=2*DSIZE){
        PUT(HDRP(bp), PACK(asize, 3));
        // PUT(FTRP(bp), PACK(asize, 1));
        PUT(HDRP(NEXT_BLKP(bp)), PACK(ori_size-asize, 2));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(ori_size-asize, 0));
        move_node(bp, NEXT_BLKP(bp));
    }
    else{
        erase_node(bp);
        PUT(HDRP(bp), PACK(ori_size, 3));
        PUT(HDRP(NEXT_BLKP(bp)), GET(HDRP(NEXT_BLKP(bp)))|2);
        // PUT(FTRP(bp), PACK(ori_size, 3));
    }
}

/*
balance fit explict link improved
Results for mm malloc:
   valid  util   ops    secs      Kops  trace
 * yes    99%    4805  0.000094  50860 ./traces/amptjp.rep
 * yes    99%    5032  0.000082  61650 ./traces/cccp.rep
 * yes   100%   14400  0.000077 187620 ./traces/coalescing-bal.rep
   yes   100%      15  0.000000  80705 ./traces/corners.rep
 * yes    99%    5683  0.000092  61744 ./traces/cp-decl.rep
 * yes    89%     118  0.000001 126273 ./traces/hostname.rep
 * yes    93%   19405  0.000178 108774 ./traces/login.rep
 * yes    92%     372  0.000003 135222 ./traces/ls.rep
   yes    96%      17  0.000000  95174 ./traces/malloc-free.rep
   yes    94%      10  0.000000  67130 ./traces/malloc.rep
 * yes    90%    1494  0.000014 109650 ./traces/perl.rep
 * yes    93%    4800  0.000486   9869 ./traces/random.rep
 * yes    95%     147  0.000001 130327 ./traces/rm.rep
   yes   100%      12  0.000000  80000 ./traces/short2.rep
 * yes    78%   57716  0.001281  45048 ./traces/boat.rep
 * yes    91%     200  0.000002 123431 ./traces/lrucd.rep
 * yes    86%  100000  0.002448  40855 ./traces/alaska.rep
 * yes    92%     200  0.000001 145109 ./traces/nlydf.rep
 * yes    92%     200  0.000002 128518 ./traces/qyqyc.rep
 * yes    97%     200  0.000001 155998 ./traces/rulsr.rep
16     92.86%  214772  0.004763  45093

best fit explict link
Results for mm malloc:
   valid  util   ops    secs      Kops  trace
 * yes    99%    4805  0.000297  16153 ./traces/amptjp.rep
 * yes   100%    5032  0.000324  15517 ./traces/cccp.rep
 * yes   100%   14400  0.000398  36201 ./traces/coalescing-bal.rep
   yes   100%      15  0.000001  19845 ./traces/corners.rep
 * yes    99%    5683  0.000384  14791 ./traces/cp-decl.rep
 * yes    87%     118  0.000005  26060 ./traces/hostname.rep
 * yes    91%   19405  0.000843  23007 ./traces/login.rep
 * yes    91%     372  0.000015  25542 ./traces/ls.rep
   yes    96%      17  0.000001  25893 ./traces/malloc-free.rep
   yes    93%      10  0.000001  19918 ./traces/malloc.rep
 * yes    87%    1494  0.000071  21151 ./traces/perl.rep
 * yes    96%    4800  0.003104   1546 ./traces/random.rep
 * yes    94%     147  0.000006  25726 ./traces/rm.rep
   yes   100%      12  0.000001  22760 ./traces/short2.rep
 * yes    56%   57716  0.360654    160 ./traces/boat.rep
 * yes    91%     200  0.000008  25548 ./traces/lrucd.rep
 * yes    83%  100000  0.007338  13629 ./traces/alaska.rep
 * yes    92%     200  0.000008  26299 ./traces/nlydf.rep
 * yes    92%     200  0.000008  25495 ./traces/qyqyc.rep
 * yes    97%     200  0.000007  29299 ./traces/rulsr.rep
balance
Results for mm malloc:
   valid  util   ops    secs      Kops  trace
 * yes    97%    4805  0.000142  33948 ./traces/amptjp.rep
 * yes    95%    5032  0.000135  37146 ./traces/cccp.rep
 * yes   100%   14400  0.000210  68518 ./traces/coalescing-bal.rep
   yes   100%      15  0.000000  44938 ./traces/corners.rep
 * yes    98%    5683  0.000181  31410 ./traces/cp-decl.rep
 * yes    87%     118  0.000002  52744 ./traces/hostname.rep
 * yes    91%   19405  0.000389  49924 ./traces/login.rep
 * yes    91%     372  0.000006  57687 ./traces/ls.rep
   yes    96%      17  0.000000  47633 ./traces/malloc-free.rep
   yes    93%      10  0.000000  38667 ./traces/malloc.rep
 * yes    87%    1494  0.000030  49032 ./traces/perl.rep
 * yes    90%    4800  0.000434  11048 ./traces/random.rep
 * yes    94%     147  0.000003  52416 ./traces/rm.rep
   yes   100%      12  0.000000  41379 ./traces/short2.rep
 * yes    56%   57716  0.001736  33255 ./traces/boat.rep
 * yes    84%     200  0.000003  58315 ./traces/lrucd.rep
 * yes    89%  100000  0.002909  34370 ./traces/alaska.rep
 * yes    80%     200  0.000004  55898 ./traces/nlydf.rep
 * yes    87%     200  0.000003  57188 ./traces/qyqyc.rep
 * yes    75%     200  0.000003  58362 ./traces/rulsr.rep
16     87.40%  214772  0.006192  34684

first fit
Results for mm malloc:
   valid  util   ops    secs      Kops  trace
 * yes    94%    4805  0.000141  34098 ./traces/amptjp.rep
 * yes    94%    5032  0.000123  40766 ./traces/cccp.rep
 * yes   100%   14400  0.000197  73139 ./traces/coalescing-bal.rep
   yes   100%      15  0.000000  42899 ./traces/corners.rep
 * yes    96%    5683  0.000212  26844 ./traces/cp-decl.rep
 * yes    87%     118  0.000002  53704 ./traces/hostname.rep
 * yes    90%   19405  0.000372  52154 ./traces/login.rep
 * yes    91%     372  0.000006  57619 ./traces/ls.rep
   yes    96%      17  0.000000  46997 ./traces/malloc-free.rep
   yes    93%      10  0.000000  36943 ./traces/malloc.rep
 * yes    86%    1494  0.000029  50738 ./traces/perl.rep
 * yes    88%    4800  0.000402  11942 ./traces/random.rep
 * yes    94%     147  0.000003  52740 ./traces/rm.rep
   yes   100%      12  0.000000  39501 ./traces/short2.rep
 * yes    56%   57716  0.001718  33595 ./traces/boat.rep
 * yes    84%     200  0.000003  60593 ./traces/lrucd.rep
 * yes    80%  100000  0.002394  41775 ./traces/alaska.rep
 * yes    77%     200  0.000003  58350 ./traces/nlydf.rep
 * yes    81%     200  0.000003  60543 ./traces/qyqyc.rep
 * yes    75%     200  0.000003  62052 ./traces/rulsr.rep
16     85.69%  214772  0.005613  38264

*/

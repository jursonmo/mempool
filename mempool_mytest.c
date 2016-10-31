#include <stdio.h>
/*
#include <linux/spinlock.h>
#include <linux/kernel.h>
#include <linux/ratelimit.h>
*/
struct mempool {
	//spinlock_t lock;
	void *head;
	void **tail;
	int nr_used;
	int nr_free;
	const char *name;
};

static __inline void
mempool_init(struct mempool *pool, const char *name, int nr_used)
{
	//spin_lock_init(&pool->lock);
	pool->head = NULL;
	pool->tail = &pool->head;
	pool->nr_used = nr_used;
	pool->nr_free = 0;
	pool->name = name;
}

static __inline void *
mempool_get(struct mempool *pool)
{
	void *data;

	//spin_lock_bh(&pool->lock);
	data = pool->head;
	if (data != NULL) {
		pool->head = *(void **)data;
		if (pool->head == NULL) {
			pool->tail = &pool->head;
		}
		pool->nr_used++;
		pool->nr_free--;
	} else {
	//	pr_warn_ratelimited("mempool oom: %s, nr_used: %d, nr_free: %d\n",
	//						pool->name, pool->nr_used, pool->nr_free);
	printf("null\n");
        }
	//spin_unlock_bh(&pool->lock);
	return data;
}

static __inline void
mempool_put(struct mempool *pool, void *data)
{
	//spin_lock_bh(&pool->lock);
	*(void **)data = NULL;
	*pool->tail = data;
	pool->tail = (void **)data;
	pool->nr_used--;
	pool->nr_free++;
	//spin_unlock_bh(&pool->lock);
}

struct st {
   int a;
   int b;
};

int main(){
   struct mempool mypool;
   mempool_init(&mypool,"mjw",10);
   struct st mydate[10];
  // memset(mydate,0,sizeof(mydate));
   int i ; 
   for (i = 0 ;i < 10 ;i++)
   {
	mydate[i].a = i;
        mempool_put(&mypool,(void *)&mydate[i]);    
   }
   struct st *date1 = (struct st *)mempool_get(&mypool);
   struct st *date2 = (struct st *)mempool_get(&mypool);
   date1->a = 1;date2->a=2;
   
   printf("date1.a=%d,date2.a=%d\n",date1->a,date2->a);
   mempool_put(&mypool,date2);

   printf("date1.a=%d,date2.a=%d\n",date1->a,date2->a);
}

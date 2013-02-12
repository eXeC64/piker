#ifndef CONFIG_H
#define CONFIG_H

/* Memory on this RPi */
#define SYS_MEM_TOTAL 512

/* Memory given to the GPU */
#define SYS_MEM_GPU    64

/* The memory we actually get to use */
#define SYS_MEM (SYS_MEM_TOTAL - SYS_MEM_GPU)

#endif /*CONFIG_H*/

/**
 * @nombre   mkmodulo.c
 * @autores  Choquevilca & Sosa Ludueña
 * @Descripcion: El siguiente programa realiza la creacion de un manejador de
 * dispositivos de tipo caracter, el cual se carga el el modulo de kernel
  */

#include <linux/init.h>            
#include <linux/module.h>          
#include <linux/device.h>          
#include <linux/kernel.h>         
#include <linux/fs.h>             
#include <asm/uaccess.h>          
#define  DEVICE_NAME "mkmodulo2"    
#define  CLASS_NAME  "mkm2"        

MODULE_LICENSE("GPL");            
MODULE_AUTHOR("Choquevilca & Sosa");    
MODULE_DESCRIPTION("Linux char driver desencriptacion");  
MODULE_VERSION("1.0");            
#define BUFFER 1000
static int    majorNumber;                  //- Almacena el número de dispositivo que es determinado automáticamente
static char   message[BUFFER] = {0};           //- Memoria para la cadena que se pasa desde el espacio de usuario
static short  size_of_message;              //- Se utiliza para recordar el tamaño de la cadena almacenada
static int    numberOpens = 0;              //- Cuenta el número de veces que se abre el dispositivo
static struct class*  mkmoduloClass  = NULL; //- puntero de estructura de clase de controlador de dispositivo
static struct device* mkmoduloDevice = NULL; //- puntero de estructura del dispositivo del controlador del dispositivo


static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);


static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};

   static int __init mkmodulo2_init(void){
   printk(KERN_INFO "mkmodulo2: Inicializando el mkmodulo2 \n");

   
   majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
   if (majorNumber<0){
      printk(KERN_ALERT "mkmodulo2: no pudo registrar un número importante\n");
      return majorNumber;
   }
   printk(KERN_INFO "mkmodulo2: ha sido registrado correctamente con el número principal %d\n", majorNumber);

   
   mkmoduloClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(mkmoduloClass)){                // Comprueba si hay errores
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Error al registrar la clase de dispositivo\n");
      return PTR_ERR(mkmoduloClass);          
   }
   printk(KERN_INFO "mkmodulo2Device: clase de dispositivo registrada correctamente \n");

   
   mkmoduloDevice = device_create(mkmoduloClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   if (IS_ERR(mkmoduloDevice)){               
      class_destroy(mkmoduloClass);           
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "Error al crear el dispositivo\n");
      return PTR_ERR(mkmoduloDevice);
   }
   printk(KERN_INFO "mkmodulo2Device: clase de dispositivo creada correctamente\n"); 
   return 0;
}


   static void __exit mkmodulo2_exit(void){
   device_destroy(mkmoduloClass, MKDEV(majorNumber, 0));     // elimina el dispositivo
   class_unregister(mkmoduloClass);                          // anula el registro de la clase de dispositivo
   class_destroy(mkmoduloClass);                             // elimina la clase de dispositivo
   unregister_chrdev(majorNumber, DEVICE_NAME);             // anula el registro del numero principal
   printk(KERN_INFO "mkmodulo2Device: el modulo se esta cerrando!\n");
}


static int dev_open(struct inode *inodep, struct file *filep){
   numberOpens++;
   printk(KERN_INFO "mkmodulo2: se ha abierto el dispositivo %d tiempo(s)\n", numberOpens);
   return 0;
}


static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
   int error_count = 0;
   
   error_count = copy_to_user(buffer, message, size_of_message);

   if (error_count==0){            
      printk(KERN_INFO "mkmodulo2Device: se ha enviado %d caracteres al usuario\n", size_of_message);
      return (size_of_message=0);  
   }
   else {
      printk(KERN_INFO "mkmodulo2Device: Error al enviar %d caracteres al usuario\n", error_count);
      return -EFAULT;             
   }
}


static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
   sprintf(message, "%s(%zu letras)", buffer, len);   
   size_of_message = strlen(message);                 
   printk(KERN_INFO "mkmodulo2Device: ha recibido %zu caracteres del usuario\n", len);
   return len;
}


static int dev_release(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "mkmodulo2Device: El dispositivo se ha cerrado\n");
   return 0;
}

module_init(mkmodulo2_init);
module_exit(mkmodulo2_exit);

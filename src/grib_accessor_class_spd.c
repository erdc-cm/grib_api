/**
* Copyright 2005-2007 ECMWF
*
* Licensed under the GNU Lesser General Public License which
* incorporates the terms and conditions of version 3 of the GNU
* General Public License.
* See LICENSE and gpl-3.0.txt for details.
*/

#include "grib_api_internal.h"
/*
   This is used by make_class.pl

   START_CLASS_DEF
   CLASS      = accessor
   SUPER      = grib_accessor_class_long
   IMPLEMENTS = unpack_long;pack_long
   IMPLEMENTS = init;dump
   IMPLEMENTS = next_offset
   IMPLEMENTS = byte_count
   IMPLEMENTS = value_count
   IMPLEMENTS = byte_offset
   IMPLEMENTS = update_size
   MEMBERS    = const char* numberOfBits
   MEMBERS    = const char* numberOfElements

   END_CLASS_DEF

 */

/* START_CLASS_IMP */

/*

Don't edit anything between START_CLASS_IMP and END_CLASS_IMP
Instead edit values between START_CLASS_DEF and END_CLASS_DEF
or edit "accessor.class" and rerun ./make_class.pl

*/

static int pack_long(grib_accessor*, const long* val,size_t *len);
static int unpack_long(grib_accessor*, long* val,size_t *len);
static long byte_count(grib_accessor*);
static long byte_offset(grib_accessor*);
static long next_offset(grib_accessor*);
static long value_count(grib_accessor*);
static void dump(grib_accessor*, grib_dumper*);
static void init(grib_accessor*,const long, grib_arguments* );
static void init_class(grib_accessor_class*);
static void update_size(grib_accessor*,size_t);

typedef struct grib_accessor_spd {
    grib_accessor          att;
/* Members defined in gen */
/* Members defined in long */
/* Members defined in spd */
	const char* numberOfBits;
	const char* numberOfElements;
} grib_accessor_spd;

extern grib_accessor_class* grib_accessor_class_long;

static grib_accessor_class _grib_accessor_class_spd = {
    &grib_accessor_class_long,                      /* super                     */
    "spd",                      /* name                      */
    sizeof(grib_accessor_spd),  /* size                      */
    0,                           /* inited */
    &init_class,                 /* init_class */
    &init,                       /* init                      */
    0,                  /* post_init                      */
    0,                    /* free mem                       */
    &dump,                       /* describes himself         */
    &next_offset,                /* get length of section     */
    &value_count,                /* get number of values      */
    &byte_count,                 /* get number of bytes      */
    &byte_offset,                /* get offset to bytes           */
    0,            /* get native type               */
    0,                /* get sub_section                */
    0,               /* grib_pack procedures long      */
    0,               /* grib_pack procedures long      */
    &pack_long,                  /* grib_pack procedures long      */
    &unpack_long,                /* grib_unpack procedures long    */
    0,                /* grib_pack procedures double    */
    0,              /* grib_unpack procedures double  */
    0,                /* grib_pack procedures string    */
    0,              /* grib_unpack procedures string  */
    0,                 /* grib_pack procedures bytes     */
    0,               /* grib_unpack procedures bytes   */
    0,            /* pack_expression */
    0,              /* notify_change   */
    &update_size,                /* update_size   */
    0,            /* preferred_size   */
    0,                    /* resize   */
    0,      /* nearest_smaller_value */
    0,                       /* next accessor    */
    0,                    /* compare vs. another accessor   */
    0,     /* unpack only ith value          */
    0,     /* unpack a subarray         */
    0,             		/* clear          */
};


grib_accessor_class* grib_accessor_class_spd = &_grib_accessor_class_spd;


static void init_class(grib_accessor_class* c)
{
	c->get_native_type	=	(*(c->super))->get_native_type;
	c->sub_section	=	(*(c->super))->sub_section;
	c->pack_missing	=	(*(c->super))->pack_missing;
	c->is_missing	=	(*(c->super))->is_missing;
	c->pack_double	=	(*(c->super))->pack_double;
	c->unpack_double	=	(*(c->super))->unpack_double;
	c->pack_string	=	(*(c->super))->pack_string;
	c->unpack_string	=	(*(c->super))->unpack_string;
	c->pack_bytes	=	(*(c->super))->pack_bytes;
	c->unpack_bytes	=	(*(c->super))->unpack_bytes;
	c->pack_expression	=	(*(c->super))->pack_expression;
	c->notify_change	=	(*(c->super))->notify_change;
	c->preferred_size	=	(*(c->super))->preferred_size;
	c->resize	=	(*(c->super))->resize;
	c->nearest_smaller_value	=	(*(c->super))->nearest_smaller_value;
	c->next	=	(*(c->super))->next;
	c->compare	=	(*(c->super))->compare;
	c->unpack_double_element	=	(*(c->super))->unpack_double_element;
	c->unpack_double_subarray	=	(*(c->super))->unpack_double_subarray;
	c->clear	=	(*(c->super))->clear;
}

/* END_CLASS_IMP */


static long byte_count(grib_accessor* a){
	return a->length;
}

static long compute_byte_count(grib_accessor* a){
  grib_accessor_spd* self = (grib_accessor_spd*)a;
  long numberOfBits;
  long numberOfElements;
  int ret=0;

  ret=grib_get_long(a->parent->h,self->numberOfBits,&numberOfBits);
  if (ret) {
		grib_context_log(a->parent->h->context,GRIB_LOG_ERROR,
			"%s unable to get %s to compute size",a->name,self->numberOfBits);
		return 0;
  }

  ret=grib_get_long(a->parent->h,self->numberOfElements,&numberOfElements);
  if (ret) {
		grib_context_log(a->parent->h->context,GRIB_LOG_ERROR,
			"%s unable to get %s to compute size",a->name,self->numberOfElements);
		return 0;
  }
  numberOfElements++;

  return (numberOfBits*numberOfElements+7)/8;
}


static void init(grib_accessor* a, const long len , grib_arguments* args )
{
	grib_accessor_spd* self = (grib_accessor_spd*)a;
	int n=0;
	self->numberOfBits=grib_arguments_get_name(a->parent->h,args,n++);
	self->numberOfElements=grib_arguments_get_name(a->parent->h,args,n++);
	a->length = compute_byte_count(a);
}

static void dump(grib_accessor* a, grib_dumper* dumper)
{
  grib_dump_long(dumper,a,NULL);
}

static int    unpack_long   (grib_accessor* a, long* val, size_t *len)
{
	grib_accessor_spd* self = (grib_accessor_spd*)a;
	int i;
	int ret=0;
	long pos = a->offset*8;
	long rlen = value_count(a);
	long numberOfBits = 0;

	if(*len < rlen)
	{
		grib_context_log(a->parent->h->context, GRIB_LOG_ERROR, 
			" wrong size (%ld) for %s it contains %d values ",*len, a->name , rlen);
		*len = 0;
		return GRIB_ARRAY_TOO_SMALL;
	}

	ret=grib_get_long(a->parent->h,self->numberOfBits,&numberOfBits);
	if (ret) return ret;

	for (i=0;i<rlen-1;i++) 
		val[i] =  grib_decode_unsigned_long(a->parent->h->buffer->data,  &pos,  numberOfBits);

	val[rlen-1] =  grib_decode_signed_longb(a->parent->h->buffer->data,  &pos,  numberOfBits);

	*len = rlen;

	return GRIB_SUCCESS;
}

static int    pack_long   (grib_accessor* a, const long* val, size_t *len)
{
	grib_accessor_spd* self = (grib_accessor_spd*)a;
	int ret = 0;
	long off = 0;
	long numberOfBits=0;
	size_t buflen  = 0;
	unsigned char *buf = NULL;
	unsigned long i = 0;
	unsigned long rlen = value_count(a);

	if(*len != rlen)
	{
	  ret=grib_set_long(a->parent->h,self->numberOfElements,(*len)-1);
	}

	ret=grib_get_long(a->parent->h,self->numberOfBits,&numberOfBits);
	if (ret) return ret;

	buflen = compute_byte_count(a);
	buf = grib_context_malloc_clear(a->parent->h->context,buflen);

	for(i=0; i < rlen-1;i++) {
		grib_encode_unsigned_longb(buf, val[i] ,  &off,  numberOfBits);
	}

	grib_encode_signed_longb(buf, val[rlen-1] ,  &off,  numberOfBits);

	grib_buffer_replace(a, buf, buflen,1,1);

	grib_context_free(a->parent->h->context,buf);

	*len=rlen;
	return ret;

}

static long value_count(grib_accessor* a)
{
  grib_accessor_spd* self = (grib_accessor_spd*)a;
  int ret;
  long numberOfElements;

  ret=grib_get_long(a->parent->h,self->numberOfElements,&numberOfElements);
  if (ret) {
		grib_context_log(a->parent->h->context,GRIB_LOG_ERROR,
			"%s unable to get %s to compute size",a->name,self->numberOfElements);
		return 0;
  }
  numberOfElements++;

  return numberOfElements;
}

static long byte_offset(grib_accessor* a){
  return a->offset;
}

static void update_size(grib_accessor* a,size_t s)
{
  a->length = s;
}

static long next_offset(grib_accessor* a){
  return byte_offset(a)+a->length;
}


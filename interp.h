/*
conventions : the declarations using mixed case always refer to elements of the python binding.
    the names of methods are prefixed with the name of their class (ex: Buffer_getname for Buffer.getname())
*/
 
#ifndef __TE_PYTHON_BINDINGS__
#define __TE_PYTHON_BINDINGS__
 
#include <Python.h>
#include <structmember.h>
#include "buffer.h"

static 	PyObject *pName, *pModule, *pDict, *pFunc;
 
void init_interp(void);

PyObject *Te_notifyprintf(PyObject *self, PyObject *args);

/* the methods of the te module */
static PyMethodDef TeMethods[] = {
// {"system", spam_system, METH_VARARGS,
// "Execute a shell command."},
	{"notifyprintf", Te_notifyprintf, METH_VARARGS, "Print a message in the minibuffer"},
	{NULL, NULL, 0, NULL} /* Sentinel */
};

/*
  Definition of the buffer object type.
 */
 
typedef struct {
  PyObject_HEAD
  struct te_buffer *buf;
} TeBuffer;
 
int Buffer_init(TeBuffer *self, PyObject *args, PyObject *kwds);
void Buffer_dealloc(TeBuffer *self);
PyObject *Buffer_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
 
PyObject *Buffer_getname(TeBuffer *self, void *closure);
int Buffer_setname(TeBuffer *self, PyObject *value, void *closure);
 
PyObject *Buffer_getpoint(TeBuffer *self, void *closure);
int Buffer_setpoint(TeBuffer *self, PyObject *value, void *closure);
 
PyObject *Buffer_getcontents(TeBuffer *self, void *closure);
int Buffer_setcontents(TeBuffer *self, PyObject *value, void *closure);


static PyMemberDef Buffer_members[] = {
	{NULL} /* Sentinel */
};
 
static PyMethodDef Buffer_methods[] = {
	{NULL} /* Sentinel */
};
 
static PyGetSetDef Buffer_getsetters[] = {
	{"point",
	 (getter)Buffer_getpoint, (setter)Buffer_setpoint,
	 "point of a buffer",
	 NULL},
 
	{"name",
	 (getter)Buffer_getname, (setter)Buffer_setname,
	 "name of a buffer",
	 NULL},
 
	{"contents",
	 (getter)Buffer_getcontents, (setter)Buffer_setcontents,
	 "contents of a buffer",
	 NULL},
 
	{NULL}
};
 
static PyTypeObject binding_BufferType = {
	PyObject_HEAD_INIT(NULL)
	0, /*ob_size*/
	"te.Buffer", /*tp_name*/
	sizeof(TeBuffer), /*tp_basicsize*/
	0, /*tp_itemsize*/
	0, /*tp_dealloc*/
	0, /*tp_print*/
	0, /*tp_getattr*/
	0, /*tp_setattr*/
	0, /*tp_compare*/
	0, /*tp_repr*/
	0, /*tp_as_number*/
	0, /*tp_as_sequence*/
	0, /*tp_as_mapping*/
	0, /*tp_hash */
	0, /*tp_call*/
	0, /*tp_str*/
	0, /*tp_getattro*/
	0, /*tp_setattro*/
	0, /*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT, /*tp_flags*/
	"A representation of a buffer in memory.", /* tp_doc */
	0, /* tp_traverse */
	0, /* tp_clear */
	0, /* tp_richcompare */
	0, /* tp_weaklistoffset */
	0, /* tp_iter */
	0, /* tp_iternext */
	Buffer_methods, /* tp_methods */
	Buffer_members, /* tp_members */
	Buffer_getsetters, /* tp_getset */
	0, /* tp_base */
	0, /* tp_dict */
	0, /* tp_descr_get */
	0, /* tp_descr_set */
	0, /* tp_dictoffset */
	(initproc)Buffer_init, /* tp_init */
	0, /* tp_alloc */
	Buffer_new, /* tp_new */
 
};
 
 
PyMODINIT_FUNC init_python(void);
 
#endif
 

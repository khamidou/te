#include "python-bindings.h"

PyMODINIT_FUNC init_python(void)
{
	Py_Initialize();
	
	if (PyType_Ready(&binding_BufferType) < 0)
		return;

	PyObject *m = Py_InitModule3("te", TeMethods,
				     "te main python api.");

	if (m == NULL)
		return;

	Py_INCREF(&binding_BufferType);
	PyModule_AddObject(m, "Buffer", (PyObject *)&binding_BufferType);

}

PyObject *Buffer_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	TeBuffer *self;

	self = (TeBuffer *)type->tp_alloc(type, 0);
	if (self != NULL) {
		self->buf = alloc_and_insert_buffer();
		if (self->buf == NULL)
		{
			Py_DECREF(self);
			return NULL;
		}

		self->buf->pyBuffer = (PyObject *) self;
	}

	return (PyObject *)self;
}

void Buffer_dealloc(TeBuffer *self)
{
	if (self == NULL)
		return;

	free_buffer(self->buf);
	self->ob_type->tp_free((PyObject*)self);
}

int Buffer_init(TeBuffer *self, PyObject *args, PyObject *kwds)
{
	PyObject *tmp;

	static char *kwlist[] = {"name", /* "file" */ NULL};

	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|S", kwlist, 
					  &tmp))
		return -1; 


 	if (self != NULL)
		self->buf->name = PyString_AsString(tmp);

	Py_XDECREF(tmp);

}

PyObject *Buffer_getname(TeBuffer *self, void *closure)
{
	if (self->buf == NULL)
		return NULL;

	PyObject *s = PyString_FromString(self->buf->name);
	Py_INCREF(s);
	return s;
}

int Buffer_setname(TeBuffer *self, PyObject *value, void *closure)
{
	if (self->buf == NULL)
		return -1;

	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot change buffer name");
		return -1;
	}

	char *s = PyString_AsString(value);
	free(self->buf->name);
	self->buf->name = strdup(s);
}

PyObject *Buffer_getpoint(TeBuffer *self, void *closure)
{
	if (self->buf == NULL)
		return NULL;

	PyObject *i = PyInt_FromLong((long) self->buf->point);
	Py_INCREF(i);
	return i;
}

int Buffer_setpoint(TeBuffer *self, PyObject *value, void *closure)
{
	if (self->buf == NULL)
		return -1;

	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Invalid object");
		return -1;
	}

	if (!PyInt_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "Parameter is not of integer type");
		return -1;
	}

	self->buf->point = (int) PyInt_AsLong(value);
}

PyObject *Buffer_getcontents(TeBuffer *self, void *closure)
{
	if (self->buf == NULL)
		return NULL;

	char *cstr = bstr2cstr(self->buf->contents, '\0');
	PyObject *s = PyString_FromString(cstr);

	free(cstr);

	Py_INCREF(s);
	return s;
}

int Buffer_setcontents(TeBuffer *self, PyObject *value, void *closure)
{
	if (self->buf == NULL)
		return -1;

	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Invalid object");
		return -1;
	}

	if (!PyInt_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "Parameter is not of integer type");
		return -1;
	}

	self->buf->point = (int) PyInt_AsLong(value);
}

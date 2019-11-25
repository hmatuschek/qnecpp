#include "object.h"
#include <list>


/* ********************************************************************************************* *
 * Implementation of GC
 * ********************************************************************************************* */
GC *GC::_singleton = nullptr;

GC::GC()
  : _objects()
{
  // pass...
}

GC::~GC() {
  for (std::unordered_map<Object *, size_t>::iterator elm=_objects.begin(); elm!=_objects.end(); elm++)
    delete elm->first;
  _objects.clear();
}

GC *
GC::get() {
  if (nullptr == _singleton)
    _singleton = new GC();
  return _singleton;
}

void
GC::ref(Object *obj) {
  std::unordered_map<Object *, size_t>::iterator elm = _objects.find(obj);
  if (_objects.end() == elm)
    _objects[obj] = 1;
  else
    elm->second++;
}

void
GC::unref(Object *obj) {
  std::unordered_map<Object *, size_t>::iterator elm = _objects.find(obj);
  if (_objects.end() == elm)
    return;
  if (elm->second)
    elm->second--;
  if (elm->second)
    return;
  run();
}

void
GC::run() {
  // Mark all referenced objects
  for (std::unordered_map<Object *, size_t>::iterator elm=_objects.begin(); elm!=_objects.end(); elm++)
    if (elm->second)
      elm->first->mark();
  // Collect all unmarked objects
  std::list<Object *> unreachable;
  for (std::unordered_map<Object *, size_t>::iterator elm=_objects.begin(); elm!=_objects.end(); elm++)
    if (! elm->first->isMarked())
      unreachable.push_back(elm->first);
  // Remove & delete all unmarked objects
  for (std::list<Object *>::iterator o=unreachable.begin(); o!=unreachable.end(); o++) {
    _objects.erase(*o);
    //delete *o;
  }
  // Unmark all remaining objects:
  for (std::unordered_map<Object *, size_t>::iterator elm=_objects.begin(); elm!=_objects.end(); elm++)
    elm->first->unmark();
}

/* ********************************************************************************************* *
 * Implementation of Object
 * ********************************************************************************************* */
Object::Object()
  : _marked(false)
{
  // Register object with GC
  GC::get()->ref(this);
}

Object::~Object() {
  // pass...
}

void
Object::mark() {
  _marked = true;
}

void
Object::unmark() {
  _marked = false;
}

bool
Object::isMarked() const {
  return _marked;
}

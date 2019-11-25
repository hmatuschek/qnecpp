#ifndef OBJECT_H
#define OBJECT_H

#include <unordered_map>
#include <cinttypes>

class Object;


class GC
{
protected:
  GC();

public:
  ~GC();

  static GC* get();

public:
  void add(Object *obj);
  void ref(Object *obj);
  void unref(Object *obj);

protected:
  void run();

protected:
  std::unordered_map<Object *, size_t> _objects;
  static GC *_singleton;
};


class Object
{
public:
  Object();
  virtual ~Object();

  virtual void mark();
  void unmark();
  bool isMarked() const;

protected:
  bool _marked;
};

#endif // OBJECT_H

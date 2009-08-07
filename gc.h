// Copyright (C) 2009 Chris Double. All Rights Reserved.
// See the license at the end of this file
#if !defined(GC_H)
#define GC_H

#include <set>
#include <map>

// Base class for all objects that are tracked by
// the garbage collector.
class GCObject {
 public:

  // For mark and sweep algorithm. When a GC occurs
  // all live objects are traversed and mMarked is
  // set to true. This is followed by the sweep phase
  // where all unmarked objects are deleted.
  bool mMarked;

 public:
  GCObject();
  GCObject(GCObject const&);
  virtual ~GCObject();

  // Mark the object and all its children as live
  void mark();

  // Overridden by derived classes to call mark()
  // on objects referenced by this object. The default
  // implemention does nothing.
  virtual void markChildren();
};

// Wrapper for an array of bytes managed by the garbage
// collector.
class GCMemory : public GCObject {
 public:
  unsigned char* mMemory;
  int   mSize;

 public:
  GCMemory(int size);
  virtual ~GCMemory();

  unsigned char* get();
  int size();
};

// Garbage Collector. Implements mark and sweep GC algorithm.
class GarbageCollector {
 public:
  // A collection of all active heap objects.
  typedef std::set<GCObject*> ObjectSet;
  ObjectSet mHeap;

  // Collection of objects that are scanned for garbage.
  ObjectSet mRoots;

  // Pinned objects
  typedef std::map<GCObject*, unsigned int> PinnedSet;
  PinnedSet mPinned;

  // Global garbage collector object
  static GarbageCollector GC;

 public:
  // Perform garbage collection. If 'verbose' is true then
  // GC stats will be printed to stdout.
  void collect(bool verbose = false);

  // Add a root object to the collector.
  void addRoot(GCObject* root);

  // Remove a root object from the collector.
  void removeRoot(GCObject* root);

  // Pin an object so it temporarily won't be collected. 
  // Pinned objects are reference counted. Pinning it
  // increments the count. Unpinning it decrements it. When
  // the count is zero then the object can be collected.
  void pin(GCObject* o);
  void unpin(GCObject* o);

  // Add an heap allocated object to the collector.
  void addObject(GCObject* o);

  // Remove a heap allocated object from the collector.
  void removeObject(GCObject* o);

  // Go through all objects in the heap, unmarking the live
  // objects and destroying the unreferenced ones.
  void sweep(bool verbose);

  // Number of live objects in heap
  int live();
};

#endif
// Copyright (C) 2009 Chris Double. All Rights Reserved.
// The original author of this code can be contacted at: chris.double@double.co.nz
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// DEVELOPERS AND CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

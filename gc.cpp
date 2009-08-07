// Copyright (C) 2009 Chris Double. All Rights Reserved.
// See the license at the end of this file
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "gc.h"

using namespace std;

// GCObject
GCObject::GCObject() :
  mMarked(false) {
  GarbageCollector::GC.addObject(this);
}

GCObject::GCObject(GCObject const&) :
  mMarked(false) {
  GarbageCollector::GC.addObject(this);
}

GCObject::~GCObject() {
}

void GCObject::mark() {
  if (!mMarked) {
    mMarked = true;
    markChildren();
  }
}

void GCObject::markChildren() {
}

// GCMemory
GCMemory::GCMemory(int size) : mSize(size) {
  mMemory = new unsigned char[size];
}

GCMemory::~GCMemory() {
  delete [] mMemory;
}

unsigned char* GCMemory::get() {
  return mMemory;
}

int GCMemory::size() {
  return mSize;
}

// GarbageCollector
GarbageCollector GarbageCollector::GC;

void GarbageCollector::collect(bool verbose) {
  using namespace boost::posix_time;
  unsigned int start = (microsec_clock::universal_time() - ptime(min_date_time)).total_milliseconds();

  // Mark root objects
  for (ObjectSet::iterator it = mRoots.begin();
       it != mRoots.end();
       ++it)
    (*it)->mark();

  // Mark pinned objects
  for (PinnedSet::iterator it = mPinned.begin();
       it != mPinned.end();
       ++it)
    (*it).first->mark();

  if (verbose) {
    cout << "Roots: " << mRoots.size() << endl;
    cout << "Pinned: " << mPinned.size() << endl;
    cout << "GC: " << mHeap.size() << " objects in heap" << endl;
  }

  sweep(verbose);

  if (verbose) {
    unsigned int end = (microsec_clock::universal_time() - ptime(min_date_time)).total_milliseconds();
    cout << "GC: " << (end-start) << " milliseconds" << endl;
  }
}

void GarbageCollector::addRoot(GCObject* root) {
  mRoots.insert(root);
}

void GarbageCollector::removeRoot(GCObject* root) {
  mRoots.erase(root);
}

void GarbageCollector::pin(GCObject* o) {
  PinnedSet::iterator it = mPinned.find(o);
  if (it == mPinned.end()) {
    mPinned.insert(make_pair(o, 1));
  }
  else {
    (*it).second++;
  }
}

void GarbageCollector::unpin(GCObject* o) {
  PinnedSet::iterator it = mPinned.find(o);
  assert(it != mPinned.end());

  if (--((*it).second) == 0) 
    mPinned.erase(it);
}

void GarbageCollector::addObject(GCObject* o) {
  mHeap.insert(o);
}

void GarbageCollector::removeObject(GCObject* o) {
  mHeap.erase(o);
}

void GarbageCollector::sweep(bool verbose) {
  unsigned int live = 0;
  unsigned int dead = 0;
  unsigned int total = 0;
  vector<ObjectSet::iterator> erase;
  for (ObjectSet::iterator it = mHeap.begin();
       it != mHeap.end();
       ++it) {
    GCObject* p = *it;
    total++;
    if (p->mMarked) {
      p->mMarked = false;
      ++live;
    }
    else {
      erase.push_back(it);
    } 
  }
  dead = erase.size();
  for (vector<ObjectSet::iterator>::iterator it = erase.begin();
       it != erase.end();
       ++it) {
    GCObject* p = **it;
    mHeap.erase(*it);
    delete p;
  }
  if (verbose) {
    cout << "GC: " << live << " objects live after sweep" << endl;
    cout << "GC: " << dead << " objects dead after sweep" << endl;
  }
}

int GarbageCollector::live() {
  return mHeap.size();
}

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

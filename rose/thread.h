#ifndef ROSE_THREAD_H
#define ROSE_THREAD_H

#include "stack.h"
#include "file.h"

class Text;
class Module;
class Debug;

#define STACK_SIZE		256

// Thread - execution stream
class Thread {
	public:
		Thread();
		~Thread();

		// Causes thread run module code, starting with constructor
		void run(Module& module);

		// Thread procedure stack
		Stack stack;

		// Returns reference to thread current text segment
		Text& text() const { return *current_text; }

		// Thread files
		File files[16];

		Module& current() const { return *current_module; }
		void set_module(Module& module);
	private:
		Thread(const Thread&);

		Module *current_module;
		Text *current_text;

	friend Debug& operator<<(Debug& stream, const Thread& th);
};

Debug& operator<<(Debug& stream, const Thread& th);

#endif


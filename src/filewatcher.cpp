#include "filewatcher.h"

FileWatcher::FileWatcher(const string& absolutePath, void (*cb)(void))
: file(absolutePath), callback(cb) {

}

FileWatcher::~FileWatcher() {
    delete ctxDesc;
}

void FileWatcher::startWatching() {
    ctxDesc = new ctx_desc;
    ctxDesc->len = 0;
    ctxDesc->size = 2;
    ctxDesc->paths = new char*[ctxDesc->size];
    ctxDesc->watcher = this;

    CFMutableArrayRef path = CFArrayCreateMutable(NULL, 1, NULL);
    CFStringRef pathStr = CFStringCreateWithCString(NULL, file.c_str(), kCFStringEncodingUTF8);
    CFArrayAppendValue(path, pathStr);

    FSEventStreamContext ctx = { 0, ctxDesc, NULL, NULL, NULL };
    FSEventStreamRef stream;
    FSEventStreamCreateFlags flags = kFSEventStreamCreateFlagFileEvents;

    stream = FSEventStreamCreate(NULL, &eventCallback, &ctx, path, kFSEventStreamEventIdSinceNow, 0, flags);

    FSEventStreamScheduleWithRunLoop(stream, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
    FSEventStreamStart(stream);

    CFRunLoopRun();
}

void FileWatcher::processEvent() const {
    if(callback != NULL) {
        (*callback)();
    }
}

string FileWatcher::watchedFile() const {
    return file;
}

void eventCallback(ConstFSEventStreamRef streamRef,
    void *ctx,
    size_t count,
    void *paths,
    const FSEventStreamEventFlags flags[],
    const FSEventStreamEventId ids[]) {

    for (size_t i = 0; i < count; i++) {
        printf("%llu \n", ids[i]);
    }

    ctx_desc *ctxDesc = (ctx_desc *)ctx;
    ctxDesc->watcher->processEvent();
}

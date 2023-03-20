cmake_minimum_required(VERSION 3.15)
project(hev-socks5-server)

set(CMAKE_CXX_STANDARD 17)

enable_language(C ASM)

add_compile_definitions(ENABLE_PTHREAD=1)

set(root "${CMAKE_CURRENT_LIST_DIR}")

add_executable(hev-socks5-server

        ${root}/src/core/src/hev-object.c
        ${root}/src/core/src/hev-socks5-client-tcp.c
        ${root}/src/core/src/hev-socks5-client-udp.c
        ${root}/src/core/src/hev-socks5-client.c
        ${root}/src/core/src/hev-socks5-logger.c
        ${root}/src/core/src/hev-socks5-misc.c
        ${root}/src/core/src/hev-socks5-server.c
        ${root}/src/core/src/hev-socks5-tcp.c
        ${root}/src/core/src/hev-socks5-udp.c
        ${root}/src/core/src/hev-socks5.c
        ${root}/src/hev-config.c
        ${root}/src/hev-main.c
        ${root}/src/hev-socket-factory.c
        ${root}/src/hev-socks5-proxy.c
        ${root}/src/hev-socks5-session.c
        ${root}/src/hev-socks5-worker.c
#        ${root}/src/misc/hev-list.c
        ${root}/src/misc/hev-logger.c
        ${root}/src/misc/hev-misc.c
        ${root}/third-part/hev-task-system/src/kern/aide/hev-task-aide.c
        ${root}/third-part/hev-task-system/src/kern/core/hev-task-system-schedule.c
        ${root}/third-part/hev-task-system/src/kern/core/hev-task-system.c
        ${root}/third-part/hev-task-system/src/kern/io/hev-task-io-reactor-epoll.c
        ${root}/third-part/hev-task-system/src/kern/io/hev-task-io-reactor-kqueue.c
        ${root}/third-part/hev-task-system/src/kern/io/hev-task-io-reactor.c
        ${root}/third-part/hev-task-system/src/kern/itc/hev-task-channel-select.c
        ${root}/third-part/hev-task-system/src/kern/itc/hev-task-channel.c
        ${root}/third-part/hev-task-system/src/kern/sync/hev-task-cond.c
        ${root}/third-part/hev-task-system/src/kern/sync/hev-task-mutex.c
        ${root}/third-part/hev-task-system/src/kern/task/hev-task-call.c
        ${root}/third-part/hev-task-system/src/kern/task/hev-task-executer.c
        ${root}/third-part/hev-task-system/src/kern/task/hev-task-stack-heap.c
        ${root}/third-part/hev-task-system/src/kern/task/hev-task-stack-mmap.c
        ${root}/third-part/hev-task-system/src/kern/task/hev-task.c
        ${root}/third-part/hev-task-system/src/kern/time/hev-task-timer-kevent.c
        ${root}/third-part/hev-task-system/src/kern/time/hev-task-timer-timerfd.c
        ${root}/third-part/hev-task-system/src/kern/time/hev-task-timer.c
        ${root}/third-part/hev-task-system/src/lib/dns/hev-task-dns-proxy.c
        ${root}/third-part/hev-task-system/src/lib/dns/hev-task-dns.c
        ${root}/third-part/hev-task-system/src/lib/io/basic/hev-task-io.c
        ${root}/third-part/hev-task-system/src/lib/io/buffer/hev-circular-buffer.c
        ${root}/third-part/hev-task-system/src/lib/io/pipe/hev-task-io-pipe.c
        ${root}/third-part/hev-task-system/src/lib/io/poll/hev-task-io-poll.c
        ${root}/third-part/hev-task-system/src/lib/io/socket/hev-task-io-socket.c
        ${root}/third-part/hev-task-system/src/lib/list/hev-list.c
        ${root}/third-part/hev-task-system/src/lib/misc/hev-debugger.c
        ${root}/third-part/hev-task-system/src/lib/rbtree/hev-rbtree-cached.c
        ${root}/third-part/hev-task-system/src/lib/rbtree/hev-rbtree.c
        ${root}/third-part/hev-task-system/src/mem/api/hev-memory-allocator-api.c
        ${root}/third-part/hev-task-system/src/mem/base/hev-memory-allocator.c
        ${root}/third-part/hev-task-system/src/mem/simple/hev-memory-allocator-simple.c
        ${root}/third-part/hev-task-system/src/mem/slice/hev-memory-allocator-slice.c
        ${root}/third-part/yaml/src/api.c
        ${root}/third-part/yaml/src/dumper.c
        ${root}/third-part/yaml/src/emitter.c
        ${root}/third-part/yaml/src/loader.c
        ${root}/third-part/yaml/src/parser.c
        ${root}/third-part/yaml/src/reader.c
        ${root}/third-part/yaml/src/scanner.c
        ${root}/third-part/yaml/src/writer.c

#        ${root}/third-part/hev-task-system/src/arch/arm/hev-task-execute-aarch64.s
#        ${root}/third-part/hev-task-system/src/arch/arm/hev-task-execute-arm.s
#        ${root}/third-part/hev-task-system/src/arch/loong/hev-task-execute-la32.s
#        ${root}/third-part/hev-task-system/src/arch/loong/hev-task-execute-la64.s
#        ${root}/third-part/hev-task-system/src/arch/mips/hev-task-execute-mips32.s
#        ${root}/third-part/hev-task-system/src/arch/mips/hev-task-execute-mips64.s
#        ${root}/third-part/hev-task-system/src/arch/riscv/hev-task-execute-riscv32.s
#        ${root}/third-part/hev-task-system/src/arch/riscv/hev-task-execute-riscv64.s
#        ${root}/third-part/hev-task-system/src/arch/sw64/hev-task-execute-sw64.s
#        ${root}/third-part/hev-task-system/src/arch/x86/hev-task-execute-x86.s
        ${root}/third-part/hev-task-system/src/arch/x86/hev-task-execute-x86_64.s
        )

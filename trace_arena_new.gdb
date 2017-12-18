set breakpoint pending on
break -probe libc:memory_arena_new
set $memory_arena_new_count = 0
set pagination off
set confirm off
commands 1
#eval "set $n_thread = \"thread_n: %d\"", $_thread
#print $n_thread
#eval "set $arena_new_calls = \"memory_arena_new: %d\"", ++$memory_arena_new_count
#print $arena_new_calls
print $memory_arena_new_count++
continue
end
catch syscall exit
commands 2
eval "set $foo = \"TOTAL memory_arena_new calls: %d\"", $memory_arena_new_count
print $foo
end
run
quit

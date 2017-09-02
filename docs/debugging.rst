.. _debugging:

Piko/RT Debugging
=================


Using tmux
----------

`tmux <https://github.com/tmux/tmux>`_ is a "terminal multiplexer", it allow you
accessed and controllerd multiple terminals inside a signle terminal like this:

.. image:: https://gifyu.com/images/asciicast.json.gif

How to use tmux, please reference these pages:

* `tmux wiki <https://github.com/tmux/tmux/wiki>`_
* `A tmux Crash Course <https://robots.thoughtbot.com/a-tmux-crash-course>`_
* `A Quick and Easy Guide to tmux <http://www.hamvocke.com/blog/a-quick-and-easy-guide-to-tmux/>`_

Using OpenOCD
-------------

XXX

Using gdb scripts
-----------------

Here are some debug scripts for gdb. You can copy and paste to your `.gdbinit`,
then used in gdb.

1. Dump out bitmap scheduler active and expire queue::

      define dump_bitmap
        set $prio = 0
        while $prio < 32
          if $arg0 == 1
            p (struct thread_info *)((char *)(sched_struct.active->queue[$prio].next) - 0x28)
          end
          if $arg0 == 0
            p (struct thread_info *)((char *)(sched_struct.expire->queue[$prio].next) - 0x28)
          end
          set $prio = $prio + 1
        end
      end

   how to use::

      (gdb) dump_bitmap 1   # Dump active
      $1 = (struct thread_info *) 0x20000d2c <vsnprintf_buf+228>
      $2 = (struct thread_info *) 0x20000d34 <vsnprintf_buf+236>
      $3 = (struct thread_info *) 0x20000d3c <vsnprintf_buf+244>
      $4 = (struct thread_info *) 0x20000d44 <vsnprintf_buf+252>
      $5 = (struct thread_info *) 0x20000d4c <sched>
      $6 = (struct thread_info *) 0x20000d54 <_active+4>
      ...
      (gdb) dump_bitmap 0   # Dump expire

2. Dump out RR scheduler runqueue::

      define dump_rr
        if list_empty(&rr_runq)
          p "empty rr"
        else
          set $pos = rr_runq->next
          while $pos != &rr_runq
            p (struct thread_info *)((char *)($pos)-0x28)
            set $pos = $pos->next
          end
        end
      end

   how to use::

      (gdb) dump_rr

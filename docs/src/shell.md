# Shell

You will be dropped into a black box called a "console". You can type things in here. And the tool 
that evaluates the thing you type is a "shell". The shell built into Crei is called "kesh". 
You can see it in action by typing `help` into the console.

## Command: `echo`

The classic echo command. Echo's something to the console.

## Command: `clear`

Clear the console buffer. This command takes no arguments. Just run it as-is.

## Command: `kmsg`

Send a message to the kernel. Pretty powerful command if you ask me. This command does take an argument. 
It is meant to be used like this:

```
kmsg <MSG>
```

**Supported Messages:**

<details>
    <summary>Show Hidden</summary>

- `k22halt`: Halt the CPU.
- `k22panic`: Trigger kernel panic.
</details>

## Command: `shutdown`

Shutdown the device. Takes no arguments, just run as-is.

# Usage

&emsp;&emsp;In this comprehensive guide, I will walk you through the process of utilizing litelog.

## I. Running Litelog

&emsp;&emsp;First and foremost, we need to have two files: `litelog`, the binary application, and `litelog.ini`, which defines the application's parameters. These files should be saved in the same directory. We grant `litelog` executable permission via the `chmod` command.

&emsp;&emsp;Subsequently, use `./litelog` to start the application. You should see the following output:

```sh
Use config file: ./litelog.ini
l1_cap: 1024
l2_cap: 1024
listen_ip: 127.0.0.1
listen_port: 20000
log_path: /var/run/litelog/
log_prefix: litelog
log_lines: 65535
ctl_recv_ip: 127.0.0.1
ctl_recv_port: 20001
ctl_send_ip: 127.0.0.1
ctl_send_port: 50000
keep_log_nums: 10
```

This output displays the configuration parameters of litelog.

&emsp;&emsp;No further action is required; litelog will record all kernel logs. If you wish to send logs to litelog or modify litelog's action criteria (e.g., set log level), please refer to the following sections.

## II. Integration with Your Application

&emsp;&emsp;Please refer to the `usage/inc` directory and add litelog.h and litelog.c to your project. You can find an example of usage in `usage/src/main.c`. The basic steps are as follows:

```c
// 1. Initialize litelog (the socket)
litelog.init("Demo_Program");

// 2.1 Send logs
litelog.log.fatal("Fatal!");
litelog.log.error("Error!");
litelog.log.warning("Warning!");
litelog.log.notice("Notice!");
litelog.log.info("Information!");
litelog.log.debug("Debug!");
litelog.log.trace("Trace!");

// 2.2 Control 
litelog.ctl.change_level(LOG_LEVEL_D | LOG_LEVEL_F);
litelog.ctl.switch_page();

// 3. Release resources
litelog.exit();
```
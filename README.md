PHP Signal Handler
====================

Extension uses C <b>signal</b> function (from <signal.h>) to call previously defined callback, if appropriate signal has arrived (i.e SIGUSR1).

### Use case:

Could be used with <b>php-amqp</b> (https://github.com/pdezwart/php-amqp) to print actual worker/consumer status (ie. message stats, last operation performed etc.)
What's more it is possible to "catch" unix signal i.e. SIGTERM and then kill the process nicely (program won't stop untill all operations are completed). It is very useful if script operates on sensitive data which cannot be damaged.

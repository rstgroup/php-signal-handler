PHP Signal Handler 
====================
[![Build Status](https://travis-ci.org/rstgroup/php-signal-handler.png?branch=master)](https://travis-ci.org/rstgroup/php-signal-handler)

Extension uses C <b>signal</b> function (from <signal.h>) to call previously defined callback, if appropriate signal has arrived (i.e SIGUSR1).

### When should I use it?

Could be used with <b>[php-amqp](https://github.com/pdezwart/php-amqp)</b> to print the actual worker/consumer status (ie. message stats, last operation performed etc.) <br />
What's more it is possible to "catch" a unix signal i.e. SIGTERM and then kill the process nicely (program won't stop untill all operations are completed). It is very useful if script operates on sensitive data which cannot be damaged.

### Why don't you use pcntl_signal?
If a blocking method (such as ```AMQPQueue::consume()``` from [php-amqp](https://github.com/pdezwart/php-amqp)) is holding the processing thread,  then you won't be able to call <i>[pcntl_signal_dispatch()](http://php.net/manual/en/function.pcntl-signal-dispatch.php)</i> and trigger the callbacks registered with <i>[pcntl_signal()](http://php.net/manual/en/function.pcntl-signal.php)</i>.

### Compatibility
Extension will work only if C function ```signal``` is available (only on UNIX systems).<br/>
PHP versions supported: <br/>
* PHP 5.6
* PHP 5.5
* PHP 5.4
* PHP 5.3
* PHP 5.2

### Example
```php
<?php

function getInfo($noStatus){
	echo "getInfo called with status $noStatus\n";
}

$r = attach_signal(10, 'getInfo');
while(true){}

?>
```

Spawn the process:
```
php your_script_name.php
```

Kill the first process from the list:
```
kill -s SIGUSR1 `ps -ef | grep php | head -n1 | awk '{print $2}'`
```

After that steps you should see:
```
getInfo called with status 10
```

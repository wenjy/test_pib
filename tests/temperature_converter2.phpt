--TEST--
Test temperature_converter
--SKIPIF--
<?php if (!extension_loaded("test_pib")) print "skip"; ?>
--FILE--
<?php
echo temperature_converter(30, 2);
?>
--EXPECTF--
30.00 degrees celsius gives 86.00 degrees fahrenheit
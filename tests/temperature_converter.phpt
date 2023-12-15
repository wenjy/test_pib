--TEST--
Test temperature_converter
--SKIPIF--
<?php if (!extension_loaded("test_pib")) print "skip"; ?>
--FILE--
<?php
echo temperature_converter(70);
?>
--EXPECTF--
70.00 degrees fahrenheit gives 21.11 degrees celsius
--TEST--
Test fahrenheit_to_celsius
--SKIPIF--
<?php if (!extension_loaded("test_pib")) print "skip"; ?>
--FILE--
<?php
var_dump(multiple_fahrenheit_to_celsius([70, 60]));
?>
--EXPECTF--
array(2) {
  [0]=>
  float(21.111111111111)
  [1]=>
  float(15.555555555556)
}

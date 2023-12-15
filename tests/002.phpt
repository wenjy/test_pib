--TEST--
test_pib_test1() Basic test
--SKIPIF--
<?php
if (!extension_loaded('test_pib')) {
	echo 'skip';
}
?>
--FILE--
<?php
$ret = test_pib_test1();

var_dump($ret);
?>
--EXPECT--
The extension test_pib is loaded and working!
NULL

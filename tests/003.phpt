--TEST--
test_pib_test2() Basic test
--SKIPIF--
<?php
if (!extension_loaded('test_pib')) {
	echo 'skip';
}
?>
--FILE--
<?php
var_dump(test_pib_test2());
var_dump(test_pib_test2('PHP'));
?>
--EXPECT--
string(11) "Hello World"
string(9) "Hello PHP"

--TEST--
Check if test_pib is loaded
--SKIPIF--
<?php
if (!extension_loaded('test_pib')) {
	echo 'skip';
}
?>
--FILE--
<?php
echo 'The extension "test_pib" is available';
?>
--EXPECT--
The extension "test_pib" is available

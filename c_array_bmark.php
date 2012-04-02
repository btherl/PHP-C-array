<?php
ini_set('memory_limit', '256M');
$br = (php_sapi_name() == "cli")? "":"<br>";

if(!extension_loaded('c_array')) {
	dl('c_array.' . PHP_SHLIB_SUFFIX);
}
$module = 'c_array';
$functions = get_extension_funcs($module);
echo "Functions available in the test extension:$br\n";
foreach($functions as $func) {
    echo $func."$br\n";
}
echo "$br\n";

$array_size = 200000;

$start_mem = memory_get_usage();

function getmicrotime() {
	list($usec, $sec) = explode(" ", microtime());
	return ((float)$usec + (float)$sec);
}
function myfloat_cmp($a, $b) {
  if ($a[2] < $b[2]) { return -1; }
  elseif ($a[2] > $b[2]) { return 1; }
  else { return 0; }
}

$start = getmicrotime();
echo "=========== Let's test c_array_push()\n";
echo "== Creating new c array\n";
$c_arr = c_array_create();
echo "== Setting structure (as before)\n";
c_array_add_entry($c_arr, 'MyString', 'string');
c_array_add_entry($c_arr, 'MyInteger', 'integer');
c_array_add_entry($c_arr, 'MyFloat', 'float');
c_array_add_entry($c_arr, 'MyDouble', 'double');
c_array_add_entry($c_arr, 'MyOtherString', 'string');
echo "== Finalize array\n";
c_array_finalize($c_arr);

echo "== Push some data into it..\n";
for ($i = 0; $i < $array_size / 2; $i++) {
  c_array_push($c_arr, array(
    0 => 'pickles',
    1 => 1,
    2 => 2.5 + rand(0,10),
    3 => 3.5,
    4 => 'bananas',
  ));
  c_array_push($c_arr, array(
    'MyString' => 'Bunnies',
    'MyInteger' => 10,
    'MyFloat' => 2.55 + rand(0,10),
    'MyDouble' => 3.55,
    'MyOtherString' => 'Rabbits',
  ));
}
printf("%.2f seconds to create array\n", getmicrotime() - $start);
printf("Memory used: %d\n", memory_get_usage() - $start_mem);

$sort_start = getmicrotime();
echo "=========== How about sorting?\n";
c_array_sort($c_arr, 'myfloat_cmp');
printf("%.2f seconds to sort array\n", getmicrotime() - $sort_start);
printf("%.2f seconds total\n", getmicrotime() - $start);
$c_arr_mem = memory_get_usage();
printf("Memory used: %d\n", $c_arr_mem - $start_mem);



$start = getmicrotime();
$php_arr = array();
for ($i = 0; $i < $array_size / 2; $i++) {
  array_push($php_arr, array(
    0 => 'pickles',
    1 => 1,
    2 => 2.5 + rand(0,10),
    3 => 3.5,
    4 => 'bananas',
  ));
  array_push($php_arr, array(
    0 => 'Bunnies',
    1 => 10,
    2 => 2.55 + rand(0,10),
    3 => 3.55,
    4 => 'Rabbits',
  ));
}
printf("%.2f seconds to create array\n", getmicrotime() - $start);
$sort_start = getmicrotime();
usort($php_arr, 'myfloat_cmp');
printf("%.2f seconds to sort array\n", getmicrotime() - $sort_start);
printf("%.2f seconds total\n", getmicrotime() - $start);
$php_arr_mem = memory_get_usage();
printf("Memory used: %d\n", $php_arr_mem - $c_arr_mem);

?>

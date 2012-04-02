<?php
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


echo "=========== Structure\n";
echo "== Creating new c array\n";
$c_arr = c_array_create();
echo "== Adding string entry\n";
c_array_add_entry($c_arr, 'MyString', 'string');
echo "== Adding integer entry\n";
c_array_add_entry($c_arr, 'MyInteger', 'integer');
echo "== Adding float entry\n";
c_array_add_entry($c_arr, 'MyFloat', 'float');
echo "== Adding double entry\n";
c_array_add_entry($c_arr, 'MyDouble', 'double');
echo "== Adding string entry\n";
c_array_add_entry($c_arr, 'MyOtherString', 'string');
echo "== Adding entry with invalid type foo\n";
c_array_add_entry($c_arr, 'name', 'foo');
echo "== Checking structure of array\n";
print_r(c_array_get_structure($c_arr));
echo "== c_array_fetch_max_index()\n";
$max_index = c_array_fetch_max_index($c_arr);
print "Got max index of $max_index\n";

echo "=========== Add data\n";
echo "== Try to add data before finalizing\n";
c_array_set($c_arr, 0, 'foo', 'bar');
echo "== Finalize array\n";
c_array_finalize($c_arr);
echo "== Try to add after finalizing\n";
c_array_add_entry($c_arr, 'MyIllegalString', 'string');
echo "== Add data out of entry index range\n";
c_array_set($c_arr, 0, 999, 'bar');
echo "== Add data\n";
c_array_set($c_arr, 0, 0, 'foo');
c_array_set($c_arr, 0, 'MyInteger', 5);
c_array_set($c_arr, 0, 2, 5.5);
c_array_set($c_arr, 0, 3, 6.6);
c_array_set($c_arr, 0, 'MyOtherString', 'bar');

echo "=========== Fetch data\n";
echo "== Fetch data out of data range\n";
c_array_fetch($c_arr, 999, 0);
echo "== Fetch data out of entry index range\n";
c_array_fetch($c_arr, 0, 999);
echo "== Fetch data back from array: ";
print c_array_fetch($c_arr, 0, 0) . ", ";
print c_array_fetch($c_arr, 0, 1) . ", ";
print c_array_fetch($c_arr, 0, 'MyFloat') . ", ";
print c_array_fetch($c_arr, 0, 'MyDouble') . ", ";
print c_array_fetch($c_arr, 0, 4) . "\n";
echo "== Fetch as single row\n";
print_r(c_array_fetch_row($c_arr, 0));
print_r(c_array_fetch_row($c_arr, 0, C_ARRAY_NUM));
print_r(c_array_fetch_row($c_arr, 0, C_ARRAY_BOTH));
echo "== Get length: ";
print c_array_fetch_length($c_arr) . "\n";
echo "== Set length to 5 then 1\n";
c_array_set_length($c_arr, 5);
c_array_set_length($c_arr, 1);
echo "== Set length below 0\n";
c_array_set_length($c_arr, -1);

echo "============ More stuff\n";
echo "== Try adding a row at once\n";
c_array_set_row($c_arr, 1, array(
  0 => 'pickles',
  1 => 1,
  2 => 2.5,
  3 => 3.5,
  4 => 'bananas',
));
print_r(c_array_fetch_row($c_arr, 1));
c_array_set_row($c_arr, 2, array(
  'MyString' => 'Bunnies',
  'MyInteger' => 10,
  'MyFloat' => 2.55,
  'MyDouble' => 3.55,
  'MyOtherString' => 'Rabbits',
));
print_r(c_array_fetch_row($c_arr, 2));


echo "=========== Finish up\n";
echo "== Destroying the c array\n";
c_array_destroy($c_arr);

echo "=========== Let's test c_array_push()\n";
echo "== Creating new c array\n";
$c_arr = c_array_create();
echo "== Setting structure (as before)\n";
c_array_add_entry($c_arr, 'MyString', 'string');
c_array_add_entry($c_arr, 'MyInteger', 'integer');
c_array_add_entry($c_arr, 'MyFloat', 'float');
c_array_add_entry($c_arr, 'MyDouble', 'double');
c_array_add_entry($c_arr, 'MyOtherString', 'string');
echo "== Checking structure of array\n";
print_r(c_array_get_structure($c_arr));
echo "== Finalize array\n";
c_array_finalize($c_arr);

echo "== Push some data into it..\n";
for ($i = 0; $i < 1000; $i++) {
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
    'MyFloat' => 1.0 / rand(0,100000),
    'MyDouble' => 1.0 / rand(0,100000),
    'MyOtherString' => null,
  ));
}
echo "\n== Verifying data\n";
for ($i = 0; $i < 2000; $i++) {
  $data = c_array_fetch_row($c_arr, $i);
  foreach ($data as $k => $v) {
    print "$k:$v,";
  }
  print "\n";
}
echo "=========== c_array_fetch_max_index()\n";
$max_index = c_array_fetch_max_index($c_arr);
print "Got max index of $max_index\n";

echo "=========== How about sorting?\n";
c_array_sort($c_arr, 'myfloat_cmp', C_ARRAY_NUM);
echo "\n== Sorted data\n";
for ($i = 0; $i < 2000; $i++) {
  $data = c_array_fetch_row($c_arr, $i);
  foreach ($data as $k => $v) {
    print "$k:$v,";
  }
  print "\n";
}
echo "=========== Serialize the array\n";
$ser = c_array_serialize($c_arr);
print "Size: " . strlen($ser) . " bytes\n$ser\n";
$php_arr = unserialize($ser);
print_r($php_arr);
$ser = c_array_serialize($c_arr, C_ARRAY_ASSOC);
print "Size: " . strlen($ser) . " bytes\n$ser\n";
$php_arr = unserialize($ser);
print_r($php_arr);
for ($i = 0; $i < 2000; $i++) {
  $row_ser = c_array_serialize_row($c_arr, $i);
  print "Row $i: $row_ser\n";
  var_dump(unserialize($row_ser));
  $row_ser = c_array_serialize_row($c_arr, $i, C_ARRAY_ASSOC);
  print "Row $i: $row_ser\n";
  var_dump(unserialize($row_ser));
}

c_array_destroy($c_arr);
function myfloat_cmp($a, $b) {
  if ($a[2] < $b[2]) { return -1; }
  elseif ($a[2] > $b[2]) { return 1; }
  else { return 0; }
};
function myfloat_cmp_assoc($a, $b) {
  if ($a['MyFloat'] < $b['MyFloat']) { return -1; }
  elseif ($a['MyFloat'] > $b['MyFloat']) { return 1; }
  else { return 0; }
};

?>

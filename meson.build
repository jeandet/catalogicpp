project('catalogicpp', 'cpp', version:'1.0.0', license:'GPLv3', default_options : ['warning_level=3', 'cpp_std=c++17'])

catalogicpp_inc = include_directories('include')

json_dep = dependency('nlohmann_json', required : true, fallback:['nlohmann_json','nlohmann_json_dep'])
stduuid_dep = dependency('stduuid', required : true, fallback:['stduuid','stduuid_dep'])
gtest_dep = dependency('gtest', main : false, fallback : ['gtest', 'gtest_dep'])

catalogicpp_dep = declare_dependency(
  include_directories : catalogicpp_inc,
  dependencies:[json_dep, stduuid_dep]
)

foreach dir:['Catalogue', 'Event', 'Json', 'Repository']
   test_exe = executable(dir, 'tests/'+dir+'/main.cpp', dependencies:[catalogicpp_dep, gtest_dep])
   test('test-'+dir, test_exe)
endforeach


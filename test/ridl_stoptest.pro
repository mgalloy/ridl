pro ridl_stoptest_class::helper
  compile_opt strictarr
  
  a = 5
  b = 7
  help, a
end


pro ridl_stoptest_class::cleanup
  compile_opt strictarr

  a = 1
  b = 2
end


function ridl_stoptest_class::init
  compile_opt strictarr

  a = 1
  b = 2
  
  return, 1
end


pro ridl_stoptest_class__define
  compile_opt strictarr

  define = { ridl_stoptest_class, a: 0L }
end


pro ridl_stoptest_helper
  compile_opt strictarr
  
  a = 5
  b = 7
  help, a
end


pro ridl_stoptest
  compile_opt strictarr
  
  stop
  
  a = 5
  
  ridl_stoptest_helper
  call_procedure, 'ridl_stoptest_helper'
  
  o = obj_new('ridl_stoptest_class')  ; problem if you .step on this line
  o->helper
  obj_destroy, o                      ; also a problem on this line
end

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
end

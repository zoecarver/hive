
declare i8* @malloc(i32)

declare i8 @free(i8*)

declare i8 @print(i32)


declare i8 @printd(double)


define double @add(double %a, double %b) {
entry:
  %addtmp = fadd double %a, %b
  ret double %addtmp
}


define i8 @simple(double %i) {
entry:
  %0 = call i8 @printd(double %i)
  ret i8 %0
}


define i8 @main(i32 %a) {
entry:
  %x = alloca double
  %0 = sitofp i32 %a to double
  %ifcond = fcmp one double %0, 0.000000e+00
  br i1 %ifcond, label %then, label %else

then:                                             ; preds = %entry
  %1 = call i8 @print(i32 2)
  br label %end

else:                                             ; preds = %entry
  %2 = call i8 @print(i32 4)
  br label %end

end:                                              ; preds = %else, %then
  %iftmp = phi i8 [ %1, %then ], [ %2, %else ]
  br i1 true, label %then1, label %else2

then1:                                            ; preds = %end
  br label %end3

else2:                                            ; preds = %end
  br label %end3

end3:                                             ; preds = %else2, %then1
  %iftmp4 = phi double [ 5.000000e+00, %then1 ], [ 2.000000e+00, %else2 ]
  store double %iftmp4, double* %x
  %3 = load double, double* %x
  %4 = call double @add(double %3, double 2.000000e+00)
  store double %4, double* %x
  %5 = load double, double* %x
  %6 = call i8 @simple(double %5)
  ret i8 %6
}


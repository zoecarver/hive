
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
  %arr = alloca double*
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
  %5 = call i8* @malloc(i32 32)
  %6 = bitcast i8* %5 to double*
  %7 = getelementptr double, double* %6, i32 0
  store double 1.000000e+00, double* %7
  %8 = getelementptr double, double* %6, i32 1
  store double 2.000000e+00, double* %8
  %9 = getelementptr double, double* %6, i32 2
  store double 3.000000e+00, double* %9
  %10 = getelementptr double, double* %6, i32 3
  store double 4.000000e+00, double* %10
  store double* %6, double** %arr
  %11 = load double*, double** %arr
  %12 = getelementptr double, double* %11, i32 1
  %13 = load double, double* %12
  %14 = call i8 @printd(double %13)
  %15 = load double, double* %x
  %16 = call i8 @simple(double %15)
  ret i8 %16
}


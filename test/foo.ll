%array_type = type { i32, double* }
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
  %ptr = alloca double*
  %arr = alloca %array_type*
  %tmp_struct = alloca %array_type
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
  %11 = getelementptr %array_type, %array_type* %tmp_struct, i32 0, i32 0
  store i32 32, i32* %11
  %12 = getelementptr %array_type, %array_type* %tmp_struct, i32 0, i32 1
  store double* %6, double** %12
  store %array_type* %tmp_struct, %array_type** %arr
  %13 = load %array_type*, %array_type** %arr
  %14 = getelementptr %array_type, %array_type* %13, i32 0
  %15 = getelementptr %array_type, %array_type* %14, i32 1
  %16 = load %array_type, %array_type* %15
  %17 = call i8 @printd(%array_type %16)
  store double* %x, double** %ptr
  %18 = load double*, double** %ptr
  %19 = load double, double* %18
  %20 = call i8 @printd(double %19)
  %21 = load double*, double** %ptr
  %22 = getelementptr double, double* %21, i32 0
  %23 = getelementptr double, double* %22, i32 0
  %24 = load double, double* %23
  %25 = call i8 @printd(double %24)
  %26 = load double, double* %x
  %27 = call i8 @simple(double %26)
  ret i8 %27
}



declare i8* @malloc(i32)

declare i8 @free(i8*)

declare i8 @print(i32)


declare i8 @print.1(double)


define i8 @simple(double %i) {
entry:
  %0 = call i8 @print(double %i)
  ret i8 %0
}


define i8 @main(i32 %a) {
entry:
  %a1 = alloca i32
  store i32 1, i32* %a1
  br i1 false, label %then, label %else

then:                                             ; preds = %entry
  %0 = call i8 @print(i32 2)
  br label %end

else:                                             ; preds = %entry
  %1 = call i8 @print(i32 4)
  br label %end

end:                                              ; preds = %else, %then
  %iftmp = phi i8 [ %0, %then ], [ %1, %else ]
  %2 = call i8 @simple(double 5.000000e+00)
  ret i8 %2
}


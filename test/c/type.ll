; ModuleID = 'type.c'
source_filename = "type.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.14.0"

%struct.T = type { i32, double }

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @main(i32) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca %struct.T, align 8
  %5 = alloca i64, align 8
  %6 = alloca i32, align 4
  %7 = alloca double, align 8
  store i32 0, i32* %2, align 4
  store i32 %0, i32* %3, align 4
  store i64 5, i64* %5, align 8
  store i32 0, i32* %6, align 4
  store double 0.000000e+00, double* %7, align 8
  %8 = load i32, i32* %3, align 4
  %9 = icmp ne i32 %8, 0
  br i1 %9, label %10, label %13

; <label>:10:                                     ; preds = %1
  %11 = load i64, i64* %5, align 8
  %12 = trunc i64 %11 to i32
  store i32 %12, i32* %6, align 4
  br label %16

; <label>:13:                                     ; preds = %1
  %14 = load i64, i64* %5, align 8
  %15 = uitofp i64 %14 to double
  store double %15, double* %7, align 8
  br label %16

; <label>:16:                                     ; preds = %13, %10
  %17 = load i32, i32* %2, align 4
  ret i32 %17
}

attributes #0 = { noinline nounwind optnone ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sahf,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 6.0.1 (tags/RELEASE_601/final)"}

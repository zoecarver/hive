; ModuleID = 'closure.c'
source_filename = "closure.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.14.0"

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @foo(i32) #0 {
  %2 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  %3 = load i32, i32* %2, align 4
  %4 = add nsw i32 %3, 1
  ret i32 %4
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @bar(i32 (...)*) #0 {
  %2 = alloca i32 (...)*, align 8
  store i32 (...)* %0, i32 (...)** %2, align 8
  %3 = load i32 (...)*, i32 (...)** %2, align 8
  %4 = bitcast i32 (...)* %3 to i32 (i32, ...)*
  %5 = call i32 (i32, ...) %4(i32 1)
  ret i32 %5
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @main() #0 {
  %1 = alloca i32 (...)*, align 8
  store i32 (...)* bitcast (i32 (i32)* @foo to i32 (...)*), i32 (...)** %1, align 8
  %2 = load i32 (...)*, i32 (...)** %1, align 8
  %3 = call i32 @bar(i32 (...)* %2)
  ret i32 0
}

attributes #0 = { noinline nounwind optnone ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sahf,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 6.0.1 (tags/RELEASE_601/final)"}

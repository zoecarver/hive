
declare i8* @malloc(i32)

declare i8 @free(i8*)

declare i32 @print(i32)


define i32 @main(i32 %a) {
entry:
  %sizes_arr = alloca i32*
  %b = alloca i8*
  %0 = call i8* @malloc(i32 12)
  %1 = call i8* @malloc(i32 12)
  %2 = bitcast i8* %1 to i32*
  %3 = bitcast i8* %0 to i32*
  %4 = getelementptr i32, i32* %3, i32 0
  store i32 1, i32* %4
  %5 = getelementptr i32, i32* %2, i32 0
  store i32 0, i32* %5
  %6 = bitcast i8* %0 to i32*
  %7 = getelementptr i32, i32* %6, i32 1
  store i32 2000, i32* %7
  %8 = getelementptr i32, i32* %2, i32 1
  store i32 1, i32* %8
  %9 = bitcast i8* %0 to i32*
  %10 = getelementptr i32, i32* %9, i32 2
  store i32 3, i32* %10
  %11 = getelementptr i32, i32* %2, i32 2
  store i32 2, i32* %11
  store i8* %0, i8** %b
  store i32* %2, i32** %sizes_arr
  %12 = load i8*, i8** %b
  %13 = load i32*, i32** %sizes_arr
  %14 = bitcast i8* %12 to i32*
  %15 = getelementptr i32, i32* %13, i32 0
  %16 = load i32, i32* %15
  %17 = getelementptr i32, i32* %14, i32 %16
  %18 = load i32, i32* %17
  %19 = call i32 @print(i32 %18)
  ret i32 %19
}


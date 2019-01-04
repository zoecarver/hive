
declare i8* @malloc(i32)

declare i8 @free(i8*)

declare i8 @print(i32)


define i8* @makearray(i32 %middleval) {
entry:
  %out = alloca i8*
  %sizes_arr = alloca i32*
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
  store i32 %middleval, i32* %7
  %8 = getelementptr i32, i32* %2, i32 1
  store i32 1, i32* %8
  %9 = bitcast i8* %0 to i32*
  %10 = getelementptr i32, i32* %9, i32 2
  store i32 3, i32* %10
  %11 = getelementptr i32, i32* %2, i32 2
  store i32 2, i32* %11
  store i8* %0, i8** %out
  %12 = load i8*, i8** %out
  ret i8* %12
}


define i8 @main(i32 %a) {
entry:
  %b = alloca i8*
  %0 = call i8* @makearray(i32 2)
  store i8* %0, i8** %b
  %1 = load i8*, i8** %b
  %2 = bitcast i8* %1 to i32*
  %3 = getelementptr i32, i32* %2, i32 1
  %4 = load i32, i32* %3
  %5 = call i8 @print(i32 %4)
  ret i8 %5
}



declare i8* @malloc(i32)

declare i8 @free(i8*)

declare i8 @print(i32)


declare i32* @filter(i32*, i32, i32 (i32)*)


define i32 @isone(i32 %x) {
entry:
  %subtmp = sub i32 %x, 1
  ret i32 %subtmp
}


define i8 @main(i32 %a) {
entry:
  %filtered = alloca i32*
  %tofilter = alloca i32*
  %0 = call i8* @malloc(i32 28)
  %1 = bitcast i8* %0 to i32*
  %2 = getelementptr i32, i32* %1, i32 0
  store i32 1, i32* %2
  %3 = getelementptr i32, i32* %1, i32 1
  store i32 0, i32* %3
  %4 = getelementptr i32, i32* %1, i32 2
  store i32 1, i32* %4
  %5 = getelementptr i32, i32* %1, i32 3
  store i32 2, i32* %5
  %6 = getelementptr i32, i32* %1, i32 4
  store i32 6, i32* %6
  %7 = getelementptr i32, i32* %1, i32 5
  store i32 0, i32* %7
  store i32* %1, i32** %tofilter
  %8 = load i32*, i32** %tofilter
  %9 = call i32* @filter(i32* %8, i32 6, i32 (i32)* @isone)
  store i32* %9, i32** %filtered
  ret i8 0
}


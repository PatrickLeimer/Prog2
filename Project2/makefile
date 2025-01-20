# Makefile to build project with C++11 standard

build:
	g++ -std=c++11 -o project2.out src/*.cpp

clean:
	rm -f project2.out output/*.tga


# Predefined Tasks: Tasks 11-13
# Task 11: Multiply circles.tga with layer1.tga
# Task 12: Flip (rotate 180 degrees) layer1.tga
# Task 13: Subtract layer2.tga from layer1.tga

tasks: build
	@echo "Executing Task 11: Multiply circles.tga with layer1.tga"
	./project2.out output/part11.tga input/circles.tga multiply input/layer1.tga

	@echo "Executing Task 12: Flip layer1.tga"
	./project2.out output/part12.tga input/layer1.tga flip

	@echo "Executing Task 13: Subtract layer2.tga from layer1.tga"
	./project2.out output/part13.tga input/layer1.tga subtract input/layer2.tga

# Additional Predefined Tasks: Tasks 14-16 (Examples)
# Task 14: Flip car.tga four times
# Task 15: Subtract layer1.tga from car.tga, multiply with circles.tga, then flip
# Task 16: Scale blue channel by 3, red by 2, green by 0

tasks_extended: build
	@echo "Executing Task 14: Flip car.tga four times"
	./project2.out output/part14.tga input/car.tga flip flip flip flip

	@echo "Executing Task 15: Subtract layer1.tga from car.tga, multiply with circles.tga, then flip"
	./project2.out output/temp15_subtract.tga input/car.tga subtract input/layer1.tga
	./project2.out output/temp15_multiply.tga output/temp15_subtract.tga multiply input/circles.tga
	./project2.out output/part15.tga output/temp15_multiply.tga flip

	@echo "Executing Task 16: Scale blue channel by 3, red by 2, green by 0"
	./project2.out output/part16.tga input/car.tga scaleblue 3 scalered 2 scalegreen 0


.PHONY: build clean tasks tasks_extended

package main

import (
	"bufio"
	"fmt"
	"math"
	"os"
	"strconv"
	"strings"
)

type Seeds = []int
type MappingFn = func(int) int

type MapEntry struct {
	dstStart int
	srcStart int
	length   int
}

type RangeMap = []MapEntry

func grabInts(parts []string) []int {
	ints := make([]int, 0, len(parts))
	for _, part := range parts {
		n, err := strconv.Atoi(part)
		if err != nil {
			continue
		}
		ints = append(ints, n)
	}
	return ints
}

func readMapData() (Seeds, []RangeMap) {
	dataFile, _ := os.Open("data/d05.txt")
	defer dataFile.Close()
	scanner := bufio.NewScanner(dataFile)
	scanner.Split(bufio.ScanLines)

	// parse seeds
	scanner.Scan()
	line := scanner.Text()
	parts := strings.Split(line, " ")
	var seeds []int = grabInts(parts)

	// parse maps
	var rangeMaps []RangeMap
	for scanner.Scan() {
		line := scanner.Text()
		if len(strings.Trim(line, " ")) == 0 {
			continue
		}
		if !(line[0] >= '0' && line[0] <= '9') {
            rangeMaps = append(rangeMaps, RangeMap{})
		} else {
			parts := strings.Split(line, " ")
			nums := grabInts(parts)
			mapEntry := MapEntry{dstStart: nums[0], srcStart: nums[1], length: nums[2]}
            lastRangeMap := &rangeMaps[len(rangeMaps)-1]
			*lastRangeMap = append(*lastRangeMap, mapEntry)
		}
	}

	return seeds, rangeMaps
}

func (m MapEntry) isValueMapped(value int) bool {
	if value < m.srcStart || value >= m.srcStart+m.length {
		return false
	}
	return true
}

func makeMappingFn(rangeMap RangeMap) MappingFn {
	return func(srcValue int) int {
		var matchingEntry *MapEntry = nil
		for _, mapEntry := range rangeMap {
			if mapEntry.isValueMapped(srcValue) {
				matchingEntry = &mapEntry
				break
			}
		}
		if matchingEntry == nil {
			return srcValue
		}
		diff := matchingEntry.dstStart - matchingEntry.srcStart
		return srcValue + diff
	}
}

func part1LowestLocationNumber(seeds []int, rangeMaps []RangeMap) int {
    mappers := make([]MappingFn, 0, len(rangeMaps))
    for _, rangeMap := range rangeMaps {
        mappers = append(mappers, makeMappingFn(rangeMap))
    }
    lowestLocation := math.MaxInt
    for _, seed := range seeds {
        val := seed
        for _, mappingFn := range mappers {
            val = mappingFn(val)
        }
        if val < lowestLocation {
            lowestLocation = val
        }
    }
	return lowestLocation
}

func main() {
	seeds, rangeMaps := readMapData()
	fmt.Printf("Day 5 - part 1: %d\n", part1LowestLocationNumber(seeds, rangeMaps))
}

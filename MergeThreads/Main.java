package com.mickey;

public class Main
{
	private static Integer[] generateArray(int length)
	{
		Integer[] array = new Integer[length];
		for (int i = 0; i < length; i++)
		{
			array[i] = length-i-1;
		}
		return array;
	}

	private static void doAllBenchmarks(MergeSortThreads p)
	{
		Integer[] a;
		long start, stop;
		for (int i = 1; i <= 8; i*=2)
		{
			System.out.println("Testing on "+i+" threads.");
			a = generateArray(5000000);
			start = System.currentTimeMillis();
			p.parallelMergeSort(a, i);
			stop = System.currentTimeMillis();
			System.out.println(stop-start);
		}
	}

	public static void main(String[] args)
	{
		doAllBenchmarks(new MergeSortThreads());
	}
}

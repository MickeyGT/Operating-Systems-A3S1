package com.mickey;
import java.util.Arrays;

public class MergeSortThreads
{

	public void parallelMergeSort(Integer[] a, int nrThreads)
	{
		if (nrThreads == 1)
		{
			serialSort(a);
			return;
		}

		int mid = a.length / 2;

		Integer[] left = Arrays.copyOfRange(a, 0, mid);
		Integer[] right = Arrays.copyOfRange(a, mid, a.length);

		Thread leftSorter = mergeSortThread(left, nrThreads);
		Thread rightSorter = mergeSortThread(right, nrThreads);

		leftSorter.start();
		rightSorter.start();

		try
		{
			leftSorter.join();
			rightSorter.join();
		}
		catch (InterruptedException e)
		{
			e.printStackTrace();
		}

		merge(left, right, a);
	}

	private Thread mergeSortThread(Integer[] a, int nrThreads)
	{
		return new Thread()
		{
			@Override
			public void run()
			{
				parallelMergeSort(a, nrThreads / 2);
			}
		};
	}

	public void serialSort(Integer[] a)
	{
		if (a.length <= 1)
			return;

		int mid = a.length / 2;

		Integer[] left = Arrays.copyOfRange(a, 0, mid);
		Integer[] right = Arrays.copyOfRange(a, mid, a.length);
		serialSort(left);
		serialSort(right);
		merge(left, right, a);
	}

	private void merge(Integer[] a, Integer[] b, Integer[] r)
	{
		int i = 0, j = 0, k = 0;
		while (i < a.length && j < b.length)
		{
			if (a[i]<b[j])
				r[k++] = a[i++];
			else
				r[k++] = b[j++];
		}

		while (i < a.length)
			r[k++] = a[i++];

		while (j < b.length)
			r[k++] = b[j++];
	}
}

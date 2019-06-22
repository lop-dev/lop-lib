using System.Collections.Generic;
using System.IO;

public class ReadonlyDetector : UnityEditor.AssetModificationProcessor {

	 
	public static string[] OnWillSaveAssets(string[] paths)
	{
		List<string> pathsToSave = new List<string>();
 
		for (int i = 0; i < paths.Length; ++i)
		{
			FileInfo info = new FileInfo(paths[i]);
			if (info.IsReadOnly)
				UnityEditor.EditorUtility.DisplayDialog("Can not save.",
					"Sorry, but " + paths[i] + " is Read-Only! Please Save as another name.",
					"Ok");
			else
				pathsToSave.Add(paths[i]);
		}
		return pathsToSave.ToArray();
	}
}

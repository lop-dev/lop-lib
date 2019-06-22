using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.IO;
using System;
using UnityEngine.SceneManagement;

public class LightData
{
    public GameObject _obj;
    public int _index;
    public Vector4 _offset;


}

public class LightDataWrapper
{
    public List<LightData> _lightDatas = new List<LightData>();
    public bool isLOD0 = false;
}


public class RecordLightData
{
    public LightDataWrapper _lightDataWrapper = new LightDataWrapper();
    public List<LightDataWrapper> _childrens = new List<LightDataWrapper>();
}


public class BuildLightmapDataWithLOD : MonoBehaviour
{

    private static List<LightData> _unLodGameObjects = new List<LightData>();
    private static List<RecordLightData> _lodGameObjectDataList = new List<RecordLightData>();
    private static string _lightingDataAssetPath;
    [MenuItem("美术/场景工具/场景烘焙及LodUV调整",false,030101)]
    public static void BuildLightmapDataWithLODFun()
    {
        _unLodGameObjects.Clear();
        _lodGameObjectDataList.Clear();
        Lightmapping.Bake();
        BackupLightmapData();
        RecordUnLodLightmapDatas();
        RecordLODLightmapDatas();
        EnableLODGameObjects(false);
        Lightmapping.Bake();
        UpdateLightmapData();
        CorrectUnLodLightmapDatas();
        CorrectLightmapData();
        EnableLODGameObjects(true);

        LightingDataAsset _asset = AssetDatabase.LoadAssetAtPath<LightingDataAsset>(_lightingDataAssetPath);
        Lightmapping.lightingDataAsset = _asset;
        _lodGameObjectDataList.Clear();
        AssetDatabase.Refresh();
    }

    private static void BackupLightmapData()
    {

        LightingDataAsset _lightingDataAsset = Lightmapping.lightingDataAsset;
        string _path = AssetDatabase.GetAssetPath(_lightingDataAsset);
        string _mainName = System.IO.Path.GetFileNameWithoutExtension(_path);
        string _ext = System.IO.Path.GetExtension(_path);
        string _folder = System.IO.Path.GetDirectoryName(_path);
        _lightingDataAssetPath = _folder + "/" + _mainName + "_LOD" + _ext;
        AssetDatabase.CopyAsset(_path, _lightingDataAssetPath);

    }


    private static void RecordUnLodLightmapDatas()
    {
        GameObject[] _rootObjs = SceneManager.GetActiveScene().GetRootGameObjects();
        foreach (GameObject _obj in _rootObjs)
        {
            doRecord(_obj);

        }
    }

    private static void doRecord(GameObject _obj)
    {
        if (_obj.activeSelf == false)
            return;

        if (_obj.GetComponent<LODGroup>() != null)
            return;

        if (_obj.GetComponent<Renderer>() != null)
        {
            StaticEditorFlags _flags = GameObjectUtility.GetStaticEditorFlags(_obj);
            if ((_flags & StaticEditorFlags.LightmapStatic) != 0)
            {
                Renderer _render = _obj.GetComponent<Renderer>();
                LightData _lightData = new LightData();
                _lightData._obj = _obj;
                _lightData._index = _render.lightmapIndex;
                _lightData._offset = _render.lightmapScaleOffset;
                _unLodGameObjects.Add(_lightData);
            }
        }

        for (int i = 0; i < _obj.transform.childCount; i++)
        {
            doRecord(_obj.transform.GetChild(i).gameObject);
        }

    }



    private static void RecordLODLightmapDatas()
    {
        LODGroup[] _objs = FindObjectsOfType<LODGroup>();
        foreach (LODGroup _obj in _objs)
        {
            if (_obj.gameObject.activeSelf == false)
                continue;

            RecordLightData _recordLightdata = new RecordLightData();
            LOD[] _lods = _obj.GetLODs();
            for (int i = 0; i < _obj.lodCount; i++)
            {
                LOD _lod;
                Renderer[] _renderers;
                if (i == 0)
                {
                    _lod = _lods[0];
                    _renderers = _lod.renderers;
                    foreach (Renderer _render in _renderers)
                    {
                        LightData _lightData = new LightData();
                        _lightData._obj = _render.gameObject;
                        //_lightData._index = _render.lightmapIndex;
                        //_lightData._offset = _render.lightmapScaleOffset;
                        _recordLightdata._lightDataWrapper._lightDatas.Add(_lightData);
                    }
                }

                _lod = _lods[i];
                _renderers = _lod.renderers;
                LightDataWrapper _lightDataWrapper = new LightDataWrapper();

                foreach (Renderer _render in _renderers)
                {
                    LightData _lightData = new LightData();
                    _lightData._obj = _render.gameObject;
                    _lightData._index = _render.lightmapIndex;
                    _lightData._offset = _render.lightmapScaleOffset;
                    _lightDataWrapper._lightDatas.Add(_lightData);

                }
                if (i == 0)
                {
                    _lightDataWrapper.isLOD0 = true;
                }
                _recordLightdata._childrens.Add(_lightDataWrapper);

            }
            _lodGameObjectDataList.Add(_recordLightdata);
        }

    }

    private static void EnableLODGameObjects(bool _val)
    {

        foreach (RecordLightData _recordLightData in _lodGameObjectDataList)
        {
            foreach (LightDataWrapper _lightDataWrapper in _recordLightData._childrens)
            {
                if (_lightDataWrapper.isLOD0 == false)
                {
                    foreach (LightData _lightData in _lightDataWrapper._lightDatas)
                    {
                        _lightData._obj.SetActive(_val);
                    }
                }
            }
        }
    }

    private static void UpdateLightmapData()
    {
        foreach (RecordLightData _recordLightData in _lodGameObjectDataList)
        {
            LightDataWrapper _lightDataWrapper = _recordLightData._lightDataWrapper;
            foreach (LightData _lightData in _lightDataWrapper._lightDatas)
            {
                Renderer _render = _lightData._obj.GetComponent<Renderer>();
                _lightData._index = _render.lightmapIndex;
                _lightData._offset = _render.lightmapScaleOffset;

            }

        }
    }

    private static void CorrectUnLodLightmapDatas()
    {
        string _tmpPath = _lightingDataAssetPath.Substring(6);
        FileStream _fs = new FileStream(Application.dataPath + _tmpPath, FileMode.Open);
        byte[] _bytes = new byte[_fs.Length];
        _fs.Read(_bytes, 0, (int)_fs.Length);
        foreach (LightData _lightData in _unLodGameObjects)
        {
            Renderer _render = _lightData._obj.GetComponent<Renderer>();
            int _tIdx = _render.lightmapIndex;
            float _tx = _render.lightmapScaleOffset.x;
            float _ty = _render.lightmapScaleOffset.y;
            float _toffsetx = _render.lightmapScaleOffset.z;
            float _toffsety = _render.lightmapScaleOffset.w;
            byte[] _txValues = BitConverter.GetBytes(_tx);
            byte[] _tyValues = BitConverter.GetBytes(_ty);
            byte[] _toffsetxValues = BitConverter.GetBytes(_toffsetx);
            byte[] _toffsetyValues = BitConverter.GetBytes(_toffsety);
            //index
            byte[] _tIdxValues = BitConverter.GetBytes(_tIdx);
            _tIdxValues[2] = 255;
            _tIdxValues[3] = 255;
            byte[] _ttotalBuffer = new byte[20];
            Buffer.BlockCopy(_tIdxValues, 0, _ttotalBuffer, 0, 4);
            Buffer.BlockCopy(_txValues, 0, _ttotalBuffer, 4, 4);
            Buffer.BlockCopy(_tyValues, 0, _ttotalBuffer, 8, 4);
            Buffer.BlockCopy(_toffsetxValues, 0, _ttotalBuffer, 12, 4);
            Buffer.BlockCopy(_toffsetyValues, 0, _ttotalBuffer, 16, 4);

            /////////////////////////////////////////////////////////////////////
            int _rIdx = _lightData._index;
            float _rx = _lightData._offset.x;
            float _ry = _lightData._offset.y;
            float _roffsetx = _lightData._offset.z;
            float _roffsety = _lightData._offset.w;

            byte[] _rxValues = BitConverter.GetBytes(_rx);
            byte[] _ryValues = BitConverter.GetBytes(_ry);
            byte[] _roffsetxValues = BitConverter.GetBytes(_roffsetx);
            byte[] _roffsetyValues = BitConverter.GetBytes(_roffsety);
            //index
            byte[] _rIdxValues = BitConverter.GetBytes(_rIdx);
            _rIdxValues[2] = 255;
            _rIdxValues[3] = 255;

            byte[] _rtotalBuffer = new byte[20];
            Buffer.BlockCopy(_rIdxValues, 0, _rtotalBuffer, 0, 4);
            Buffer.BlockCopy(_rxValues, 0, _rtotalBuffer, 4, 4);
            Buffer.BlockCopy(_ryValues, 0, _rtotalBuffer, 8, 4);
            Buffer.BlockCopy(_roffsetxValues, 0, _rtotalBuffer, 12, 4);
            Buffer.BlockCopy(_roffsetyValues, 0, _rtotalBuffer, 16, 4);
            int _v = FindBuffer(_bytes, _rtotalBuffer);
            if (_v != -1)
            {
                _fs.Seek(_v, SeekOrigin.Begin);
                _fs.Write(_ttotalBuffer, 0, 20);
            }

        }
        _fs.Flush();
        _fs.Close();

    }



    private static void CorrectLightmapData()
    {
        string _tmpPath = _lightingDataAssetPath.Substring(6);
        FileStream _fs = new FileStream(Application.dataPath + _tmpPath, FileMode.Open);
        byte[] _bytes = new byte[_fs.Length];
        _fs.Read(_bytes, 0, (int)_fs.Length);
        foreach (RecordLightData _recordLightData in _lodGameObjectDataList)
        {
            LightDataWrapper _lightDataWrapper = _recordLightData._lightDataWrapper;
            for (int i = 0; i < _lightDataWrapper._lightDatas.Count; i++)
            {
                LightData _lightData = _lightDataWrapper._lightDatas[i];
                int _tIdx = _lightData._index;
                float _tx = _lightData._offset.x;
                float _ty = _lightData._offset.y;
                float _toffsetx = _lightData._offset.z;
                float _toffsety = _lightData._offset.w;
                byte[] _txValues = BitConverter.GetBytes(_tx);
                byte[] _tyValues = BitConverter.GetBytes(_ty);
                byte[] _toffsetxValues = BitConverter.GetBytes(_toffsetx);
                byte[] _toffsetyValues = BitConverter.GetBytes(_toffsety);
                //index
                byte[] _tIdxValues = BitConverter.GetBytes(_tIdx);
                _tIdxValues[2] = 255;
                _tIdxValues[3] = 255;
                byte[] _ttotalBuffer = new byte[20];
                Buffer.BlockCopy(_tIdxValues, 0, _ttotalBuffer, 0, 4);
                Buffer.BlockCopy(_txValues, 0, _ttotalBuffer, 4, 4);
                Buffer.BlockCopy(_tyValues, 0, _ttotalBuffer, 8, 4);
                Buffer.BlockCopy(_toffsetxValues, 0, _ttotalBuffer, 12, 4);
                Buffer.BlockCopy(_toffsetyValues, 0, _ttotalBuffer, 16, 4);
                foreach (LightDataWrapper _lightDataWrapperChild in _recordLightData._childrens)
                {
                    if (i > _lightDataWrapperChild._lightDatas.Count)
                        continue;
                    LightData _lightDataChild = _lightDataWrapperChild._lightDatas[i];
                    Renderer _render = _lightDataChild._obj.GetComponent<Renderer>();

                    _render.lightmapIndex = _lightData._index;
                    _render.lightmapScaleOffset = _lightData._offset;

                    int _rIdx = _lightDataChild._index;
                    float _rx = _lightDataChild._offset.x;
                    float _ry = _lightDataChild._offset.y;
                    float _roffsetx = _lightDataChild._offset.z;
                    float _roffsety = _lightDataChild._offset.w;

                    byte[] _rxValues = BitConverter.GetBytes(_rx);
                    byte[] _ryValues = BitConverter.GetBytes(_ry);
                    byte[] _roffsetxValues = BitConverter.GetBytes(_roffsetx);
                    byte[] _roffsetyValues = BitConverter.GetBytes(_roffsety);
                    //index
                    byte[] _rIdxValues = BitConverter.GetBytes(_rIdx);
                    _rIdxValues[2] = 255;
                    _rIdxValues[3] = 255;

                    byte[] _rtotalBuffer = new byte[20];
                    Buffer.BlockCopy(_rIdxValues, 0, _rtotalBuffer, 0, 4);
                    Buffer.BlockCopy(_rxValues, 0, _rtotalBuffer, 4, 4);
                    Buffer.BlockCopy(_ryValues, 0, _rtotalBuffer, 8, 4);
                    Buffer.BlockCopy(_roffsetxValues, 0, _rtotalBuffer, 12, 4);
                    Buffer.BlockCopy(_roffsetyValues, 0, _rtotalBuffer, 16, 4);
                    int _v = FindBuffer(_bytes, _rtotalBuffer);
                    if (_v != -1)
                    {
                        _fs.Seek(_v, SeekOrigin.Begin);
                        _fs.Write(_ttotalBuffer, 0, 20);
                    }
                }
            }
        }

        _fs.Flush();
        _fs.Close();


    }

    private static int FindBuffer(byte[] srcBytes, byte[] searchBytes)
    {
        if (srcBytes == null) { return -1; }
        if (searchBytes == null) { return -1; }
        if (srcBytes.Length == 0) { return -1; }
        if (searchBytes.Length == 0) { return -1; }
        if (srcBytes.Length < searchBytes.Length) { return -1; }
        for (int i = 0; i < srcBytes.Length - searchBytes.Length; i++)
        {
            if (srcBytes[i] == searchBytes[0])
            {
                if (searchBytes.Length == 1) { return i; }
                bool flag = true;
                for (int j = 1; j < searchBytes.Length; j++)
                {
                    if (srcBytes[i + j] != searchBytes[j])
                    {
                        flag = false;
                        break;
                    }
                }
                if (flag) { return i; }
            }
        }
        return -1;
    }

}
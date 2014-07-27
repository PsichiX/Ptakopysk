using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ZasuvkaPtakopyskaExtender.Editors
{
    public class AssetPropertyEditor : EnumPropertyEditor, IAssetsModelRequired
    {
        public enum AssetType
        {
            Texture,
            Shader,
            Sound,
            Music,
            Font
        }

        private static readonly string NONE = "";

        private SceneModel.Assets m_assetsModel;
        private AssetType m_assetType;

        public SceneModel.Assets AssetsModel { get { return m_assetsModel; } set { m_assetsModel = value; UpdateValuesSource(); } }
        public AssetType AssetsType { get { return m_assetType; } set { m_assetType = value; UpdateValuesSource(); } }
        public object SelectedAsset
        {
            get
            {
                if (m_assetsModel == null)
                    return null;

                if (m_assetType == AssetType.Texture && m_assetsModel.textures != null)
                    return m_assetsModel.textures.Find(item => item.id == Value);
                else if (m_assetType == AssetType.Shader && m_assetsModel.shaders != null)
                    return m_assetsModel.shaders.Find(item => item.id == Value);
                else if (m_assetType == AssetType.Sound && m_assetsModel.sounds != null)
                    return m_assetsModel.sounds.Find(item => item.id == Value);
                else if (m_assetType == AssetType.Music && m_assetsModel.musics != null)
                    return m_assetsModel.musics.Find(item => item.id == Value);
                else if (m_assetType == AssetType.Font && m_assetsModel.fonts != null)
                    return m_assetsModel.fonts.Find(item => item.id == Value);
                else
                    return null;
            }
            set
            {
                if (m_assetsModel == null)
                    return;
                
                if (m_assetType == AssetType.Texture && value is SceneModel.Assets.Texture)
                    Value = (value as SceneModel.Assets.Texture).id;
                else if (m_assetType == AssetType.Shader && value is SceneModel.Assets.Shader)
                    Value = (value as SceneModel.Assets.Shader).id;
                else if (m_assetType == AssetType.Sound && value is SceneModel.Assets.Sound)
                    Value = (value as SceneModel.Assets.Sound).id;
                else if (m_assetType == AssetType.Music && value is SceneModel.Assets.Music)
                    Value = (value as SceneModel.Assets.Music).id;
                else if (m_assetType == AssetType.Font && value is SceneModel.Assets.Font)
                    Value = (value as SceneModel.Assets.Font).id;
            }
        }

        public AssetPropertyEditor(object propertyOwner, string propertyName, SceneModel.Assets assetsModel, AssetType assetType)
            : base(propertyOwner, propertyName, null)
        {
            m_assetsModel = assetsModel;
            m_assetType = assetType;
            UpdateValuesSource();
        }

        public AssetPropertyEditor(Dictionary<string, object> properties, string propertyName, SceneModel.Assets assetsModel, AssetType assetType)
            : base(properties, propertyName, null)
        {
            m_assetsModel = assetsModel;
            m_assetType = assetType;
            UpdateValuesSource();
        }

        private void UpdateValuesSource()
        {
            List<string> values = new List<string>();
            values.Add(NONE);
            if (m_assetsModel != null)
            {
                if (m_assetType == AssetType.Texture && m_assetsModel.textures != null)
                    foreach (SceneModel.Assets.Texture item in m_assetsModel.textures)
                        values.Add(item.id);
                else if (m_assetType == AssetType.Shader && m_assetsModel.shaders != null)
                    foreach (SceneModel.Assets.Shader item in m_assetsModel.shaders)
                        values.Add(item.id);
                else if (m_assetType == AssetType.Sound && m_assetsModel.sounds != null)
                    foreach (SceneModel.Assets.Sound item in m_assetsModel.sounds)
                        values.Add(item.id);
                else if (m_assetType == AssetType.Music && m_assetsModel.musics != null)
                    foreach (SceneModel.Assets.Music item in m_assetsModel.musics)
                        values.Add(item.id);
                else if (m_assetType == AssetType.Font && m_assetsModel.fonts != null)
                    foreach (SceneModel.Assets.Font item in m_assetsModel.fonts)
                        values.Add(item.id);
            }
            ValuesSource = values.ToArray();
            UpdateEditorValue();
        }
    }
}
